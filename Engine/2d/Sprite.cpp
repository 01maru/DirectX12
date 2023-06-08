#include "Sprite.h"
#include "ConstBuffStruct.h"
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
		size = textureSize;
	//}

#pragma region VertBuff

	vertices.clear();
	vertices.resize(4);

	SetVerticesPos();
	SetVerticesUV();

	uint16_t sizePV = static_cast<uint16_t>(sizeof(vertices[0]) * vertices.size());
	VertIdxBuff::Initialize(sizePV);

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
	if (dirtyFlagPos) SetVerticesPos();
	if (dirtyFlagUV) SetVerticesUV();

	if (dirtyFlagPos || dirtyFlagUV)
	{
		dirtyFlagPos = false;
		dirtyFlagUV = false;
		
		TransferVertex();
	}

	if (dirtyFlagColor)
	{
		dirtyFlagColor = false;
		mapMaterial->color = color;
	}

	MatUpdate();
}

void Sprite::MatUpdate()
{
	mat.Update();

	if (dirtyFlagMat)
	{
		dirtyFlagMat = false;

		constMapTransform->mat = mat.GetMatWorld();
		constMapTransform->mat *= mat2D;
	}
}

void Sprite::Draw(GPipeline* pipeline)
{
	if (isInvisible) return;

	GPipeline* pipeline_ = nullptr;
	if (pipeline != nullptr) pipeline_ = pipeline;

	else					 pipeline_ = PipelineManager::GetInstance()->GetPipeline("Sprite", GPipeline::ALPHA_BLEND);
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	IASetVertIdxBuff();
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle.GetHandle()));

	colorMaterial.SetGraphicsRootCBuffView(1);
	transform.SetGraphicsRootCBuffView(2);

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
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
	TransferVertex();
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

void Sprite::SetPosition(const Vector2D& position)
{
	Vector2D trans = mat.GetTrans();

	if (trans != position)
	{
		dirtyFlagMat = true;
		mat.SetTrans(position);
	}
}

void Sprite::SetRotation(float rotation)
{
	if (mat.GetAngle() != rotation)
	{
		dirtyFlagMat = true;
		mat.SetAngle(rotation);
	}
}

void Sprite::SetAnchorPoint(const Vector2D& anchor)
{
	if (anchorPoint != anchor)
	{
		dirtyFlagPos = true;
		anchorPoint = anchor;
	}
}

void Sprite::SetSize(const Vector2D& size_)
{
	if (size != size_)
	{
		dirtyFlagPos = true;
		size = size_;
	}
}

void Sprite::SetColor(const Vector4D& color_)
{
	if (color != color_)
	{
		dirtyFlagColor = true;
		color = color_;
	}
}

void Sprite::SetTextureLeftTop(const Vector2D& leftTop)
{
	if (textureLeftTop != leftTop)
	{
		dirtyFlagUV = true;
		textureLeftTop = leftTop;
	}
}

void Sprite::SetTextureSize(const Vector2D& size_)
{
	if (textureSize != size_) 
	{
		dirtyFlagUV = true;
		textureSize = size_;
	}
}
