#include "Grass.h"
#include "TextureManager.h"

MyDirectX* Grass::dx = MyDirectX::GetInstance();
ICamera* Grass::camera = nullptr;
GPipeline* Grass::pipeline = nullptr;

void Grass::SetCamera(ICamera* camera_)
{
	Grass::camera = camera_;
}

void Grass::SetPipeline(GPipeline* pipe)
{
	pipeline = pipe;
}

void Grass::Initialize()
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
	ID3D12Device* dev = dx->GetDev();
#pragma region  ConstBuffer
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = transform->Map(0, nullptr, (void**)&constMapTransform);	//	マッピング
	assert(SUCCEEDED(result));

	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = material->Map(0, nullptr, (void**)&mapMaterial);	//	マッピング
	assert(SUCCEEDED(result));

	cbResourceDesc.Width = (sizeof(ConstBufferDataWind) + 0xFF) & ~0xFF;
	//	生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&windRes));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = windRes->Map(0, nullptr, (void**)&constMapWind);	//	マッピング
	assert(SUCCEEDED(result));
#pragma endregion
	UINT sizePV = static_cast<UINT>(sizeof(vertex) * 1);

	BuffInitialize(dev, sizePV, 1);

	wind.Initialize();
}

Grass::Grass()
{
	Initialize();
}

Grass::Grass(const Vector3D& pos_)
{
	vertex = pos_;
	Initialize();
}

void Grass::MatUpdate()
{
	wind.Update();
	constMapWind->windDir = wind.GetDir();
	constMapWind->windForce = wind.GetForce();
	constMapWind->elapsedTime = MyMath::ConvertToRad(wind.GetTime());

	constMapTransform->matBillboard = Matrix();
	if (isBillboardY) {
		constMapTransform->matBillboard = camera->GetBillboardY();
	}
	if (isBillboard) {
		constMapTransform->matBillboard = camera->GetBillboard();
	}
	constMapTransform->scale = scale;
	constMapTransform->cameraPos = camera->GetEye();
	constMapTransform->mat = camera->GetViewProj();

	mapMaterial->color = color;
}

void Grass::Draw(int handle)
{
	ID3D12GraphicsCommandList* cmdList = dx->GetCmdList();

	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle));
	cmdList->SetGraphicsRootConstantBufferView(1, material->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(3, windRes->GetGPUVirtualAddress());

	cmdList->DrawInstanced(1, 1, 0, 0);
}

void Grass::SetScale(float scale_)
{
	scale = scale_;
}

void Grass::SetPosition(const Vector3D& pos)
{
	vertex = pos;
	SetVertices();
}

void Grass::Move(const Vector3D& spd)
{
	vertex += spd;
	SetVertices();
}

void Grass::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertex);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vector3D* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < 1; i++) {
		vertMap[i] = vertex; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}
