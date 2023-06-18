#include "Particle.h"
#include "TextureManager.h"
#include "DirectX.h"
#include <cassert>

#include "ConstBuffStruct.h"

void Particle::TransferVertex()
{
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vector3D* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	vertMap = &vertex_; // 座標をコピー
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}

void Particle::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertex_);

	//	GPUメモリの値書き換えよう
	TransferVertex();
}

void Particle::Initialize()
{
	HRESULT result;
	
#pragma region  ConstBuffer

	transform_.Initialize(sizeof(CBuff::CBufferParticleTransform));
	//	定数バッファのマッピング
	result = transform_.GetResource()->Map(0, nullptr, (void**)&cTransformMap_);	//	マッピング
	assert(SUCCEEDED(result));

	colorMaterial_.Initialize(sizeof(CBuff::CBuffColorMaterial));
	//	定数バッファのマッピング
	result = colorMaterial_.GetResource()->Map(0, nullptr, (void**)&cColorMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region VertBuff

	uint16_t sizePV = static_cast<uint16_t>(sizeof(vertex_));
	VertIdxBuff::Initialize(sizePV);

#pragma endregion
}

Particle::Particle()
{
	Initialize();
}

Particle::Particle(const Vector3D& pos)
{
	vertex_ = pos;
	Initialize();
}

void Particle::MatUpdate()
{
	cTransformMap_->matBillboard = Matrix();
	//if (isBillboardY_) {
	//	cTransformMap_->matBillboard = camera->GetBillboardY();
	//}
	//if (isBillboard_) {
	//	cTransformMap_->matBillboard = camera->GetBillboard();
	//}
	cTransformMap_->scale = scale_;
	//cTransformMap_->cameraPos = camera->GetEye();
	//cTransformMap_->mat = camera->GetViewProj();

	cColorMap_->color = color_;
}

void Particle::Draw(int32_t handle)
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	//pipeline->SetGraphicsRootSignature();
	//pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	IASetVertIdxBuff();
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle));
	colorMaterial_.SetGraphicsRootCBuffView(1);
	transform_.SetGraphicsRootCBuffView(2);

	cmdList->DrawInstanced(1, 1, 0, 0);
}

void Particle::SetScale(float scale)
{
	scale_ = scale;
}

void Particle::SetPosition(const Vector3D& pos)
{
	vertex_ = pos;
	TransferVertex();
}

void Particle::Move(const Vector3D& spd)
{
	vertex_ += spd;
	TransferVertex();
}
