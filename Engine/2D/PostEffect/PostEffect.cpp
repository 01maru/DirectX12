#include "PostEffect.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include <cassert>

#include "ConstBuffStruct.h"

void PostEffect::Initialize(int32_t width, int32_t height, float weight, DXGI_FORMAT format)
{
	texture_.resize(texNum_);
	for (size_t i = 0; i < texNum_; i++)
	{
		TextureManager::GetInstance()->CreateNoneGraphTexture("postEffect", texture_[i]);
	}

#pragma region  ConstBuffer

	material_.Initialize((sizeof(CBuff::CBuffColorMaterial) + 0xFF) & ~0xFF);

	HRESULT result = material_.GetResource()->Map(0, nullptr, (void**)&cMaterialMap_);	//	マッピング
	assert(SUCCEEDED(result));
	SetColor(color_);

	weight_.Initialize((sizeof(CBuff::CBufferBlurWeight) + 0xFF) & ~0xFF);

	weights_.resize(8);
	MyMath::CalcGaussianWeightsTable(
		weights_,        // 重みの格納先
		weight          // ボケ具合。この数値が大きくなるとボケが強くなる
	);

	CBuff::CBufferBlurWeight* mapWeight = nullptr;
	result = weight_.GetResource()->Map(0, nullptr, (void**)&mapWeight);	//	マッピング
	mapWeight->weight[0] = Vector4D(weights_[0], weights_[1], weights_[2], weights_[3]);
	mapWeight->weight[1] = Vector4D(weights_[4], weights_[5], weights_[6], weights_[7]);
	assert(SUCCEEDED(result));
	weight_.GetResource()->Unmap(0, nullptr);

#pragma endregion

	vertices_.clear();
	vertices_.push_back({ {-1.0f,-1.0f,0.0f},{0,1} });
	vertices_.push_back({ {-1.0f, 1.0f,0.0f},{0,0} });
	vertices_.push_back({ { 1.0f,-1.0f,0.0f},{1,1} });
	vertices_.push_back({ { 1.0f, 1.0f,0.0f},{1,0} });

	uint32_t sizePV = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	//	インデックスデータ
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(3);
	VertIdxBuff::Initialize(sizePV, indices_);

	//	ビューポート
	viewPortSciRect_.InitializeVP(width, height, 0, 0, 0.0f, 1.0f, texNum_);
	// シザー矩形
	viewPortSciRect_.InitializeSR(0, width, 0, height, texNum_);

	auto resDesc = MyDirectX::GetInstance()->GetBackBuffDesc();
	resDesc.Format = format;
	resDesc.Width = width;
	resDesc.Height = height;
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

	for (size_t i = 0; i < texNum_; i++)
	{
		result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(texture_[i].GetResourceBuffAddress()));
	}

#pragma region RTV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = MyDirectX::GetInstance()->GetRTVHeapDesc();
	//	heap
	heapDesc.NumDescriptors = texNum_;
	result = MyDirectX::GetInstance()->GetDev()->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(rtvHeap_.ReleaseAndGetAddressOf()));

	D3D12_RENDER_TARGET_VIEW_DESC _rtvDesc = {};
	_rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	if (format == DXGI_FORMAT_R8G8B8A8_UNORM) {
		_rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}
	else {
		_rtvDesc.Format = format;
	}

	//	RTV
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	for (size_t i = 0; i < texNum_; i++)
	{
		rtvHandle_.ptr += MyDirectX::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * i;
		MyDirectX::GetInstance()->GetDev()->CreateRenderTargetView(
			texture_[i].GetResourceBuff(),
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
	for (size_t i = 0; i < texNum_; i++)
	{
		size_t incrementSize = MyDirectX::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = MyDirectX::GetInstance()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart();
		srvHandle.ptr += incrementSize * texture_[i].GetHandle();

		MyDirectX::GetInstance()->GetDev()->CreateShaderResourceView(
			texture_[i].GetResourceBuff(),
			&_srvDesc,
			srvHandle);
	}
#pragma endregion

#pragma region 深度バッファ
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = width;
	depthResourceDesc.Height = height;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//	深度地用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//	深度地のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	//	Resource生成
	result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));
	//	デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = MyDirectX::GetInstance()->GetDev()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	//	view
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	MyDirectX::GetInstance()->GetDev()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
}

void PostEffect::Draw(bool xBlur, bool yBlur, bool shadow, int32_t handle1)
{
	GPipeline* pipeline = nullptr;
	if (shadow) {
		pipeline = PipelineManager::GetInstance()->GetPipeline("PostEffectShadow", GPipeline::NONE_BLEND);
		if (xBlur) {
			pipeline = PipelineManager::GetInstance()->GetPipeline("xBlur", GPipeline::NONE_BLEND);
		}
		if (yBlur) {
			pipeline = PipelineManager::GetInstance()->GetPipeline("yBlur", GPipeline::NONE_BLEND);
		}
	}
	else {
		pipeline = PipelineManager::GetInstance()->GetPipeline("PostEffect", GPipeline::NONE_BLEND);
		if (xBlur) {
			pipeline = PipelineManager::GetInstance()->GetPipeline("luminncexBlur", GPipeline::NONE_BLEND);
		}
		if (yBlur) {
			pipeline = PipelineManager::GetInstance()->GetPipeline("luminnceyBlur", GPipeline::NONE_BLEND);
		}
	}
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertIdxBuff::IASetVertIdxBuff();

	if (xBlur == false && yBlur == false && shadow == false) {
		//	テクスチャ
		cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[0].GetHandle()));
		if (handle1 != -1) {
			cmdList->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(handle1));
		}
		else {
			cmdList->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(texture_[0].GetHandle()));
		}
		cmdList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetTextureHandle(texture_[0].GetHandle()));
		cmdList->SetGraphicsRootDescriptorTable(3, TextureManager::GetInstance()->GetTextureHandle(texture_[0].GetHandle()));
		material_.SetGraphicsRootCBuffView(4);
		if (xBlur || yBlur) 	weight_.SetGraphicsRootCBuffView(5);
	}
	else {
		//	テクスチャ
		cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[0].GetHandle()));
		material_.SetGraphicsRootCBuffView(1);
		if (xBlur || yBlur) 	weight_.SetGraphicsRootCBuffView(2);
	}

	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void PostEffect::SetColor(const Vector4D& color)
{
	cMaterialMap_->color = color;
}

void PostEffect::Setting()
{
	viewPortSciRect_.RSSetVPandSR();
}

void PostEffect::DrawLuminnce()
{
	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("Luminnce", GPipeline::NONE_BLEND);

	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertIdxBuff::IASetVertIdxBuff();
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[0].GetHandle()));
	material_.SetGraphicsRootCBuffView(1);

	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void PostEffect::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (size_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}
