#include "PostEffect.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "Input.h"

void PostEffect::Initialize(DXGI_FORMAT format)
{
	texture.resize(texNum);
	for (int i = 0; i < texNum; i++)
	{
		TextureManager::GetInstance()->CreateNoneGraphTexture("postEffect", texture[i]);
	}

	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
#pragma region  ConstBuffer
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	HRESULT result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	SetColor(color);
#pragma endregion

	vertices.clear();
	vertices.push_back({ {-1.0f,-1.0f,0.0f},{0,1} });
	vertices.push_back({ {-1.0f, 1.0f,0.0f},{0,0} });
	vertices.push_back({ { 1.0f,-1.0f,0.0f},{1,1} });
	vertices.push_back({ { 1.0f, 1.0f,0.0f},{1,0} });

	UINT sizePV = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	//	インデックスデータ
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	BuffInitialize(MyDirectX::GetInstance()->GetDev(), sizePV, sizeIB, indices, indexSize);

	//	ビューポート
	viewPort.Init(Window::window_width, Window::window_height, 0, 0, 0.0f, 1.0f, texNum);
	// シザー矩形
	scissorRect.Init(0, Window::window_width, 0, Window::window_height, texNum);

	auto resDesc_ = MyDirectX::GetInstance()->GetBackBuffDesc();
	resDesc_.Format = format;
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	float clsClr[4] = { 1.0f,1.0f,1.0f,1.0f };
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.DepthStencil.Depth = 1.0f;
	for (size_t i = 0; i < 4; i++)
	{
		clearValue.Color[i] = clsClr[i];
	}

	for (int i = 0; i < texNum; i++)
	{
		result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc_,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(texture[i].GetResourceBuffAddress()));
	}

#pragma region RTV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = MyDirectX::GetInstance()->GetRTVHeapDesc();
	//	heap
	heapDesc.NumDescriptors = texNum;
	result = MyDirectX::GetInstance()->GetDev()->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(rtvHeap.ReleaseAndGetAddressOf()));

	D3D12_RENDER_TARGET_VIEW_DESC _rtvDesc = {};
	_rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	if (format == DXGI_FORMAT_R8G8B8A8_UNORM) {
		_rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}
	else {
		_rtvDesc.Format = format;
	}

	//	RTV
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < texNum; i++)
	{
		rtvHandle_.ptr += MyDirectX::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * i;
		MyDirectX::GetInstance()->GetDev()->CreateRenderTargetView(
			texture[i].GetResourceBuff(),
			&_rtvDesc,
			rtvHandle_);
	}
#pragma endregion

#pragma region SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
	_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	_srvDesc.Format = _rtvDesc.Format;
	_srvDesc.Texture2D.MipLevels = 1;
	_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	
	//	SRV
	for (int i = 0; i < texNum; i++)
	{
		UINT incrementSize = MyDirectX::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = MyDirectX::GetInstance()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart();
		srvHandle.ptr += incrementSize * texture[i].GetHandle();

		MyDirectX::GetInstance()->GetDev()->CreateShaderResourceView(
			texture[i].GetResourceBuff(),
			&_srvDesc,
			srvHandle);
	}
#pragma endregion
}

void PostEffect::Draw()
{
	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("PostEffect", GPipeline::NONE_BLEND);

	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture[0].GetHandle()));
	cmdList->SetGraphicsRootConstantBufferView(1, material->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}

void PostEffect::SetColor(const Vector4D& color_)
{
	ConstBufferDataMaterial* mapMaterial = nullptr;
	HRESULT result = material->Map(0, nullptr, (void**)&mapMaterial);	//	マッピング
	mapMaterial->color = color_;
	assert(SUCCEEDED(result));
	material->Unmap(0, nullptr);
}

void PostEffect::Setting()
{
	viewPort.Update();

	scissorRect.Update();
}

void PostEffect::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

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
