#include "Sprite.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "Window.h"

Matrix Sprite::mat2D;

void Sprite::StaticInitialize()
{
	mat2D.Identity();
	mat2D.m[0][0] = 2.0f / Window::window_width;
	mat2D.m[1][1] = -2.0f / Window::window_height;
	mat2D.m[3][0] = -1;
	mat2D.m[3][1] = 1;
}

void Sprite::Initialize(Texture texture_)
{	
	HRESULT result;

	//if (handle_ != UINT32_MAX) {
		handle = texture_;
		AdjustTextureSize();
	//}

#pragma region VertBuff

	vertices.resize(4);

	SetVerticesPos();
	SetVerticesUV();

	UINT sizePV = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	BuffInitialize(MyDirectX::GetInstance()->GetDev(), sizePV, (int)vertices.size());

#pragma endregion

#pragma region ConstBuffer

	transform.Initialize((sizeof(CBuff::CBuffSpriteTransform) + 0xFF) & ~0xFF);
	//	定数バッファのマッピング
	result = transform.GetResource()->Map(0, nullptr, (void**)&constMapTransform);	//	マッピング
	assert(SUCCEEDED(result));


	colorMaterial.Initialize((sizeof(CBuff::CBuffColorMaterial) + 0xFF) & ~0xFF);
	//	定数バッファのマッピング
	result = colorMaterial.GetResource()->Map(0, nullptr, (void**)&mapMaterial);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion
}

void Sprite::Update()
{
	ID3D12Resource* texBuff = handle.GetResourceBuff();

	if (texBuff) {
		D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();

		float tex_left = textureLeftTop.x / resDesc_.Width;
		float tex_right = (textureLeftTop.x + textureSize.x) / resDesc_.Width;
		float tex_top = textureLeftTop.y / resDesc_.Height;
		float tex_bottom = (textureLeftTop.y + textureSize.y) / resDesc_.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
		TransferVertex();
	}

	MatUpdate();
}

void Sprite::MatUpdate()
{
	mat.Update();

	constMapTransform->mat = mat.GetMatWorld();
	constMapTransform->mat *= mat2D;

	mapMaterial->color = color;
}

void Sprite::SetTextureRect()
{
	ID3D12Resource* texBuff = handle.GetResourceBuff();

	if (texBuff) {
		D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();

		float tex_left = textureLeftTop.x / (float)resDesc_.Width;
		float tex_right = (textureLeftTop.x + textureSize.x) / (float)resDesc_.Width;
		float tex_top = textureLeftTop.y / (float)resDesc_.Height;
		float tex_bottom = (textureLeftTop.y + textureSize.y) / (float)resDesc_.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };

		// 頂点バッファに送信
		ScreenVertex* vertMap = nullptr;
		HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
		// 全頂点に対して
		for (int i = 0; i < (int)vertices.size(); i++) {
			vertMap[i] = vertices[i]; // 座標をコピー
		}
		// 繋がりを解除
		vertBuff->Unmap(0, nullptr);
	}

	MatUpdate();
}

void Sprite::SetVerticesUV()
{
	ID3D12Resource* texBuff = handle.GetResourceBuff();

	if (texBuff) {
		D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();

		float tex_left = textureLeftTop.x / (float)resDesc_.Width;
		float tex_right = (textureLeftTop.x + textureSize.x) / (float)resDesc_.Width;
		float tex_top = textureLeftTop.y / (float)resDesc_.Height;
		float tex_bottom = (textureLeftTop.y + textureSize.y) / (float)resDesc_.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
	}
}

void Sprite::SetVerticesPos()
{
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (isFlipX) {
		left = -left;
		right = -right;
	}
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,0.0f };
}

void Sprite::TransferVertex()
{

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void Sprite::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void Sprite::AdjustTextureSize()
{
	ID3D12Resource* texBuff = handle.GetResourceBuff();
	assert(texBuff);

	D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();
	textureSize.x = static_cast<float>(resDesc_.Width);
	textureSize.y = static_cast<float>(resDesc_.Height);
}

void Sprite::Draw()
{
	if (isInvisible) {
		return;
	}

	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("Sprite", GPipeline::ALPHA_BLEND);
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	BuffUpdate(MyDirectX::GetInstance()->GetCmdList());
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle.GetHandle()));

	colorMaterial.SetGraphicsRootCBuffView(1);
	transform.SetGraphicsRootCBuffView(2);

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void Sprite::DrawRect(const Vector2D& textureLeftTop_, const Vector2D& textureSize_)
{
	bool dirtyFlag = false;

	if (this->textureLeftTop != textureLeftTop_) {
		dirtyFlag = true;
		this->textureLeftTop = textureLeftTop_;
	}
	if (this->textureSize != textureSize_) {
		dirtyFlag = true;
		this->textureSize = textureSize_;
	}

	if (dirtyFlag) {
		SetTextureRect();
	}

	if (isInvisible) {
		return;
	}

	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("Sprite", GPipeline::ALPHA_BLEND);
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	BuffUpdate(MyDirectX::GetInstance()->GetCmdList());
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle.GetHandle()));
	//MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(1, material->GetGPUVirtualAddress());
	//MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	transform.SetGraphicsRootCBuffView(2);

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}
