#pragma once
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

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;

	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	ComPtr<ID3D12Fence> uploadTexFence_;
	UINT64 uploadTexFenceVal_ = 0;

	D3D12_RESOURCE_BARRIER barrierDesc_{};

	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

	ComPtr<ID3D12Resource> depthBuff_;

	//	一つだけ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
	
	//	ビューポートシザー矩形
	ViewPortScissorRect viewPortSciRect_;

private:
	void DebugLayer();

	void ScreenClear(FLOAT* clearColor, D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle);
	void ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle);
	
	void SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource = nullptr);
	void CmdListDrawAble(D3D12_RESOURCE_BARRIER& barrierDesc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter,
		D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE& dsvHandle, int32_t rtDescNum = 1, FLOAT* clearColor = nullptr);
	
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
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRVHeapForHeapStart() { return srvHeap_->GetCPUDescriptorHandleForHeapStart(); }
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRVHeapForHeapStart() { return srvHeap_->GetGPUDescriptorHandleForHeapStart(); }

	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }
	ID3D12Device* GetDev() { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList_.Get(); }
	ID3D12GraphicsCommandList* GetLoadTexCmdList() { return loadTexCmdList_.Get(); }
	D3D12_RESOURCE_DESC GetBackBuffDesc() { return backBuffers_[0]->GetDesc(); }
	D3D12_DESCRIPTOR_HEAP_DESC GetRTVHeapDesc() { return rtvHeap_->GetDesc(); }
};

