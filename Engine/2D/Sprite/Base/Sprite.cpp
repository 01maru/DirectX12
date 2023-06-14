#include "Sprite.h"
#include "ConstBuffStruct.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "Window.h"

Matrix Sprite::sMat2DTransform = Create2DTransformMatrix();

void Sprite::Initialize(Texture texture)
{
	HRESULT result;

	//if (handle_ != UINT32_MAX) {
	handle_ = texture;
	AdjustTextureSize();
	size_ = textureSize_;
	//}

#pragma region VertBuff

	vertices_.clear();
	vertices_.resize(4);

	SetVerticesPos();
	SetVerticesUV();

	uint32_t sizePV = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	VertIdxBuff::Initialize(sizePV);

#pragma endregion

#pragma region ConstBuffer

	cbTransform_.Initialize(sizeof(CBuff::CBuffSpriteTransform));
	//	定数バッファのマッピング
	result = cbTransform_.GetResource()->Map(0, nullptr, (void**)&cbTransformMat_);	//	マッピング
	assert(SUCCEEDED(result));


	cbColorMaterial_.Initialize(sizeof(CBuff::CBuffColorMaterial));
	//	定数バッファのマッピング
	result = cbColorMaterial_.GetResource()->Map(0, nullptr, (void**)&cbMaterialMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion
}

void Sprite::Update()
{
	if (dirtyFlagPos_) SetVerticesPos();
	if (dirtyFlagUV_) SetVerticesUV();

	if (dirtyFlagPos_ || dirtyFlagUV_)
	{
		dirtyFlagPos_ = false;
		dirtyFlagUV_ = false;
		
		TransferVertex();
	}

	if (dirtyFlagColor_)
	{
		dirtyFlagColor_ = false;
		cbMaterialMap_->color = color_;
	}

	MatUpdate();
}

void Sprite::MatUpdate()
{
	mat_.Update();

	if (dirtyFlagMat_)
	{
		dirtyFlagMat_ = false;

		cbTransformMat_->mat = mat_.GetMatWorld();
		cbTransformMat_->mat *= sMat2DTransform;
	}
}

void Sprite::Draw(GPipeline* pipeline)
{
	if (isInvisible_) return;

	GPipeline* pipeline_ = nullptr;
	if (pipeline != nullptr) pipeline_ = pipeline;

	else					 pipeline_ = PipelineManager::GetInstance()->GetPipeline("Sprite", GPipeline::ALPHA_BLEND);
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	IASetVertIdxBuff();
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle_.GetHandle()));

	cbColorMaterial_.SetGraphicsRootCBuffView(1);
	cbTransform_.SetGraphicsRootCBuffView(2);

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SetVerticesUV()
{
	ID3D12Resource* texBuff = handle_.GetResourceBuff();

	if (texBuff) {
		D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();

		float tex_left = textureLeftTop_.x / (float)resDesc_.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / (float)resDesc_.Width;
		float tex_top = textureLeftTop_.y / (float)resDesc_.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / (float)resDesc_.Height;

		vertices_[LB].uv = { tex_left,tex_bottom };
		vertices_[LT].uv = { tex_left,tex_top };
		vertices_[RB].uv = { tex_right,tex_bottom };
		vertices_[RT].uv = { tex_right,tex_top };
	}
}

void Sprite::SetVerticesPos()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	vertices_[LB].pos = { left,bottom,0.0f };
	vertices_[LT].pos = { left,top,0.0f };
	vertices_[RB].pos = { right,bottom,0.0f };
	vertices_[RT].pos = { right,top,0.0f };
}

void Sprite::TransferVertex()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void Sprite::SetVertices()
{
	TransferVertex();
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices_[0]);
}

void Sprite::AdjustTextureSize()
{
	ID3D12Resource* texBuff = handle_.GetResourceBuff();
	assert(texBuff);

	D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();
	textureSize_.x = static_cast<float>(resDesc_.Width);
	textureSize_.y = static_cast<float>(resDesc_.Height);
}

void Sprite::SetPosition(const Vector2D& position)
{
	Vector2D trans = mat_.GetTrans();

	if (trans != position)
	{
		dirtyFlagMat_ = true;
		mat_.SetTrans(position);
	}
}

void Sprite::SetRotation(float rotation)
{
	if (mat_.GetAngle() != rotation)
	{
		dirtyFlagMat_ = true;
		mat_.SetAngle(rotation);
	}
}

void Sprite::SetAnchorPoint(const Vector2D& anchor)
{
	if (anchorPoint_ != anchor)
	{
		dirtyFlagPos_ = true;
		anchorPoint_ = anchor;
	}
}

void Sprite::SetSize(const Vector2D& size)
{
	if (size_ != size)
	{
		dirtyFlagPos_ = true;
		size_ = size;
	}
}

void Sprite::SetColor(const Vector4D& color)
{
	if (color_ != color)
	{
		dirtyFlagColor_ = true;
		color_ = color;
	}
}

void Sprite::SetTextureLeftTop(const Vector2D& leftTop)
{
	if (textureLeftTop_ != leftTop)
	{
		dirtyFlagUV_ = true;
		textureLeftTop_ = leftTop;
	}
}

void Sprite::SetTextureSize(const Vector2D& size)
{
	if (textureSize_ != size) 
	{
		dirtyFlagUV_ = true;
		textureSize_ = size;
	}
}
