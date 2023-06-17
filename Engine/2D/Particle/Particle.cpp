#include "Particle.h"
#include "TextureManager.h"
#include "PipelineManager.h"

#include "ConstBuffStruct.h"

MyDirectX* Particle::dx = MyDirectX::GetInstance();
ICamera* Particle::camera = nullptr;
GPipeline* Particle::pipeline = nullptr;

void Particle::SetCamera(ICamera* camera_)
{
	Particle::camera = camera_;
}

void Particle::SetPipeline(GPipeline* pipe)
{
	pipeline = pipe;
}

void Particle::Initialize()
{
	HRESULT result;
	
#pragma region  ConstBuffer

	transform.Initialize(sizeof(ConstBufferDataTransform));
	//	定数バッファのマッピング
	result = transform.GetResource()->Map(0, nullptr, (void**)&cTransformMap);	//	マッピング
	assert(SUCCEEDED(result));

	colorMaterial.Initialize(sizeof(CBuff::CBuffColorMaterial));
	//	定数バッファのマッピング
	result = colorMaterial.GetResource()->Map(0, nullptr, (void**)&cColorMap);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region VertBuff

	uint16_t sizePV = static_cast<uint16_t>(sizeof(vertex));
	VertIdxBuff::Initialize(sizePV);

#pragma endregion
}

Particle::Particle()
{
	Initialize();
}

Particle::Particle(const Vector3D& pos_)
{
	vertex = pos_;
	Initialize();
}

void Particle::MatUpdate()
{
	cTransformMap->matBillboard = Matrix();
	if (isBillboardY) {
		cTransformMap->matBillboard = camera->GetBillboardY();
	}
	if (isBillboard) {
		cTransformMap->matBillboard = camera->GetBillboard();
	}
	cTransformMap->scale = scale;
	cTransformMap->cameraPos = camera->GetEye();
	cTransformMap->mat = camera->GetViewProj();

	cColorMap->color = color;
}

void Particle::Draw(int handle)
{
	ID3D12GraphicsCommandList* cmdList = dx->GetCmdList();

	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	IASetVertIdxBuff();
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle));
	colorMaterial.SetGraphicsRootCBuffView(1);
	transform.SetGraphicsRootCBuffView(2);

	cmdList->DrawInstanced(1, 1, 0, 0);
}

void Particle::SetScale(float scale_)
{
	scale = scale_;
}

void Particle::SetPosition(const Vector3D& pos)
{
	vertex = pos;
	SetVertices();
}

void Particle::Move(const Vector3D& spd)
{
	vertex += spd;
	SetVertices();
}

void Particle::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertex);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vector3D* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < 1; i++) {
		vertMap[i] = vertex; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}
