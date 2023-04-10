#pragma once
#include "Window.h"
#include "ViewPort.h"
#include "ScissorRect.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <wrl.h>
#include <chrono>
#include <thread>
#include "PostEffect.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDirectX
{
private:
	Window* win = nullptr;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device;

	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	ComPtr<ID3D12CommandQueue> cmdQueue;

	ComPtr<IDXGISwapChain4> swapChain;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};

	FLOAT clearColor[4] = {};

	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	ComPtr<ID3D12Resource> depthBuff;

	//	一つだけ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	
	//	ビューポート
	ViewPort viewPort;
	// シザー矩形
	ScissorRect scissorRect;

	std::chrono::steady_clock::time_point reference_;

private:
	void DebugLayer();

	void InitializeFPS();

	void ScreenClear(FLOAT* clearColor_, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_);
	void ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_);
	
	void SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource = nullptr);
	void CmdListDrawAble(D3D12_RESOURCE_BARRIER& barrierDesc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter,
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_, int rtDescNum = 1, FLOAT* clearColor_ = nullptr);
public:
	MyDirectX() {};
	static MyDirectX* GetInstance();
	void Initialize();
	static void DeleteInstance();
	void UpdateFPS();
	void PrevPostEffect(PostEffect* postEffect);
	void PostEffectDraw(PostEffect* postEffect);
	void PrevDraw(FLOAT* clearColor_ = nullptr);
	void PostDraw();

	//	Getter
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRVHeapForHeapStart() { return srvHeap->GetCPUDescriptorHandleForHeapStart(); }
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRVHeapForHeapStart() { return srvHeap->GetGPUDescriptorHandleForHeapStart(); }

	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap.Get(); }
	ID3D12Device* GetDev() { return device.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
	Matrix GetViewportMat() { return viewPort.Mat(); }
	D3D12_RESOURCE_DESC GetBackBuffDesc() { return backBuffers[0]->GetDesc(); }
	D3D12_DESCRIPTOR_HEAP_DESC GetRTVHeapDesc() { return rtvHeap->GetDesc(); }
};

