#include "DirectX.h"
#include "Window.h"
#include "PostEffect.h"
#include <cassert>

#pragma comment (lib, "winmm.lib")

// 対応レベルの配列
D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};

MyDirectX* MyDirectX::GetInstance()
{
	static MyDirectX instance;
	return &instance;
}

void MyDirectX::DebugLayer()
{
	//	オン
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

void MyDirectX::Initialize()
{
#ifdef _DEBUG
	DebugLayer();
#endif

#pragma region GPU列挙
#pragma region Adapter
	ComPtr<IDXGIFactory7> dxgiFactory;
	// DXGIファクトリーの生成
	HRESULT result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	//	グラボアダプタ選別
	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i].Get();
			break;
		}
	}
#pragma endregion Adapter

#pragma region Device
	//	デバイス生成
	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion Device
#pragma endregion

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	device.As(&infoQueue);

	D3D12_MESSAGE_ID denyIds[] = {
	  D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
	  D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};
	D3D12_MESSAGE_SEVERITY severities[] = {
	  D3D12_MESSAGE_SEVERITY_INFO
	};
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;

	infoQueue->PushStorageFilter(&filter);

	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif

#pragma region CmdList
	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	assert(SUCCEEDED(result));
	// コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));
	assert(SUCCEEDED(result));

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&loadTexAllocator));
	assert(SUCCEEDED(result));
	// コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		loadTexAllocator.Get(), nullptr,
		IID_PPV_ARGS(&loadTexCmdList));
	assert(SUCCEEDED(result));
#pragma endregion CmdList

#pragma region CmdQueue
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&cmdQueue));
	assert(SUCCEEDED(result));

	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&loadTexQueue));
	assert(SUCCEEDED(result));
#pragma endregion CmdQueue

#pragma region DoubleBuffering
#pragma region swapChain
	ComPtr<IDXGISwapChain1> swapChain1;
	// 設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;										//	画面幅解像度
	swapChainDesc.Height = 720;										//	画面高さ解像度
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 色情報の書式
	swapChainDesc.SampleDesc.Count = 1;								// マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				// バックバッファ用
	swapChainDesc.BufferCount = 2;									// バッファ数を2つに設定(表と裏)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(), Window::GetInstance()->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapChain1);

	swapChain1.As(&swapChain);		//	1→4に変換
	assert(SUCCEEDED(result));
#pragma endregion swapChain

#pragma region DesHeap
	// デスクリプタヒープの設定(GPUにメモリ領域確保しそれから使う)
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
#pragma endregion DesHeap

#pragma region BackBuff
	backBuffers.resize(swapChainDesc.BufferCount);
#pragma endregion BackBuff

#pragma region RTV
	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++) {
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドルを取得(先頭アドレス)
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
#pragma endregion RTV
#pragma endregion

#pragma region SRV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = rtvHeap->GetDesc();
	const size_t kMaxSRVCount = 2056;
	//	heapSETTING
	heapDesc.NumDescriptors = kMaxSRVCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	
	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(srvHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region 深度バッファ
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Window::window_width;
	depthResourceDesc.Height = Window::window_height;
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
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
	//	デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//	view
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
#pragma endregion

#pragma region fence
	// フェンスの生成
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	// フェンスの生成
	result = device->CreateFence(uploadTexFenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&uploadTexFence));
#pragma endregion fence

	//	ビューポート
	viewPort.InitializeVP(Window::window_width, Window::window_height, 0, 0, 0.0f, 1.0f);
	// シザー矩形
	viewPort.InitializeSR(0, Window::window_width, 0, Window::window_height);
}

void MyDirectX::SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource)
{
	if (pResource != nullptr) {
		desc.Transition.pResource = pResource;
	}
	desc.Transition.StateBefore = StateBefore;
	desc.Transition.StateAfter = StateAfter;
	cmdList->ResourceBarrier(1, &desc);
}

void MyDirectX::CmdListDrawAble(D3D12_RESOURCE_BARRIER& desc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_, int rtDescNum, FLOAT* clearColor_)
{
	// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
	SetResourceBarrier(desc, StateBefore, StateAfter, pResource);
#pragma endregion ReleaseBarrier
	// 2.描画先の変更
#pragma region Change
	cmdList->OMSetRenderTargets(rtDescNum, &rtvHandle_, false, &dsvHandle_);
#pragma endregion Change
	// 3.画面クリア
#pragma region ScreenClear
	if (clearColor_ == nullptr) {
		ScreenClear(rtvHandle_);
	}
	else {
		ScreenClear(clearColor_, rtvHandle_);
	}
	cmdList->ClearDepthStencilView(dsvHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion
}

void MyDirectX::PrevPostEffect(PostEffect* postEffect, FLOAT* clearColor_)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = postEffect->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	dsvHandle = postEffect->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();

	int num = postEffect->GetTextureNum();
	for (int i = 0; i < num; i++)
	{
		// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
		SetResourceBarrier(postEffect->GetResouceBarrier(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET, postEffect->GetTextureBuff(i));
#pragma endregion ReleaseBarrier
	}

	// 2.描画先の変更
#pragma region Change
	cmdList->OMSetRenderTargets(num, &rtvHandle_, true, &dsvHandle);
#pragma endregion Change

	// 3.画面クリア
#pragma region ScreenClear
	for (int i = 0; i < postEffect->GetTextureNum(); i++)
	{
		rtvHandle_.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * i;

		if (clearColor_ != nullptr) {
			ScreenClear(clearColor_, rtvHandle_);
		}
		else {
			ScreenClear(rtvHandle_);
		}
	}
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

	postEffect->Setting();

	cmdList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
}

void MyDirectX::PostEffectDraw(PostEffect* postEffect)
{
	for (int i = 0; i < postEffect->GetTextureNum(); i++)
	{
		SetResourceBarrier(postEffect->GetResouceBarrier(), D3D12_RESOURCE_STATE_RENDER_TARGET
			, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, postEffect->GetTextureBuff(i));
	}
}

void MyDirectX::PrevDraw(FLOAT* clearColor_)
{
	// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
	// バックバッファの番号を取得(2つなので0番か1番)
	UINT64 bbIndex = swapChain->GetCurrentBackBufferIndex();		//	現在のバックバッファ設定
#pragma endregion ReleaseBarrier

	// 2.描画先の変更
#pragma region Change
	// レンダーターゲットビューのハンドルを取得
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion Change

	CmdListDrawAble(barrierDesc, backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET, rtvHandle, dsvHandle, 1, clearColor_);

	viewPort.Update();
}

void MyDirectX::PostDraw()
{
	// 5.リソースバリアを戻す
#pragma region ReleaseBarrier
	SetResourceBarrier(barrierDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
#pragma endregion ReleaseBarrier

	// 命令のクローズ
#pragma region CmdClose
	HRESULT result = cmdList->Close();
	assert(SUCCEEDED(result));
	// 溜めていたコマンドリストの実行(close必須)
	ID3D12CommandList* commandLists[] = { cmdList.Get()};
	cmdQueue->ExecuteCommandLists(1, commandLists);
	// 画面に表示するバッファをフリップ(裏表の入替え)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
#pragma endregion CmdClose

#pragma region ChangeScreen
	// コマンドの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)	//	GPUの処理が完了したか判定
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// キューをクリア
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = cmdList->Reset(cmdAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
#pragma endregion ChangeScreen
}

void MyDirectX::UploadTexture()
{
	// 命令のクローズ
#pragma region CmdClose
	HRESULT result = loadTexCmdList->Close();
	assert(SUCCEEDED(result));
	// 溜めていたコマンドリストの実行(close必須)
	ID3D12CommandList* commandLists[] = { loadTexCmdList.Get() };
	loadTexQueue->ExecuteCommandLists(1, commandLists);
#pragma endregion CmdClose

#pragma region ChangeScreen
	// コマンドの実行完了を待つ
	loadTexQueue->Signal(uploadTexFence.Get(), ++uploadTexFenceVal);
	if (uploadTexFence->GetCompletedValue() != uploadTexFenceVal)	//	GPUの処理が完了したか判定
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		uploadTexFence->SetEventOnCompletion(uploadTexFenceVal, event);
		if (event != 0) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// キューをクリア
	result = loadTexAllocator->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = loadTexCmdList->Reset(loadTexAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
#pragma endregion ChangeScreen
}

void MyDirectX::ScreenClear(FLOAT* clearColor_, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_)
{
	cmdList->ClearRenderTargetView(rtvHandle_, clearColor_, 0, nullptr);
}
void MyDirectX::ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_)
{
	//FLOAT clearColor_[] = { 0.1f,0.25f, 0.5f,0.0f };
	FLOAT clearColor_[] = { 1.0f,1.0f,1.0f,1.0f };
	cmdList->ClearRenderTargetView(rtvHandle_, clearColor_, 0, nullptr);
}

