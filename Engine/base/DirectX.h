﻿#pragma once
#define NOMINMAX
#include "ViewPortScissorRect.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <vector>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class PostEffect;

class MyDirectX
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device_;

	ComPtr<ID3D12CommandAllocator> cmdAllocator_;
	ComPtr<ID3D12GraphicsCommandList> cmdList_;

	ComPtr<ID3D12CommandQueue> cmdQueue_;

	ComPtr<ID3D12CommandAllocator> loadTexAllocator_;
	ComPtr<ID3D12GraphicsCommandList> loadTexCmdList_;

	ComPtr<ID3D12CommandQueue> loadTexQueue_;

	ComPtr<IDXGISwapChain4> swapChain_;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	ComPtr<ID3D12Fence> uploadTexFence;
	UINT64 uploadTexFenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};

	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	ComPtr<ID3D12Resource> depthBuff;

	//	一つだけ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	
	//	ビューポート
	ViewPortScissorRect viewPortSciRect_;
	// シザー矩形

private:
	void DebugLayer();

	void ScreenClear(FLOAT* clearColor, D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle_);
	void ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle_);
	
	void SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource = nullptr);
	void CmdListDrawAble(D3D12_RESOURCE_BARRIER& barrierDesc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter,
		D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle_, D3D12_CPU_DESCRIPTOR_HANDLE& dsvHandle_, int32_t rtDescNum = 1, FLOAT* clearColor = nullptr);
	
	MyDirectX() {};
	~MyDirectX() {};
public:
	static MyDirectX* GetInstance();
	MyDirectX(const MyDirectX& obj) = delete;
	MyDirectX& operator=(const MyDirectX& obj) = delete;

	void Initialize();
	void PrevPostEffect(PostEffect* postEffect, FLOAT* clearColor = nullptr);
	void PostEffectDraw(PostEffect* postEffect);
	void PrevDraw(FLOAT* clearColor = nullptr);
	void PostDraw();
	void UploadTexture();

	//	Getter
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRVHeapForHeapStart() { return srvHeap->GetCPUDescriptorHandleForHeapStart(); }
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRVHeapForHeapStart() { return srvHeap->GetGPUDescriptorHandleForHeapStart(); }

	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap.Get(); }
	ID3D12Device* GetDev() { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList_.Get(); }
	ID3D12GraphicsCommandList* GetLoadTexCmdList() { return loadTexCmdList_.Get(); }
	D3D12_RESOURCE_DESC GetBackBuffDesc() { return backBuffers[0]->GetDesc(); }
	D3D12_DESCRIPTOR_HEAP_DESC GetRTVHeapDesc() { return rtvHeap->GetDesc(); }
};

