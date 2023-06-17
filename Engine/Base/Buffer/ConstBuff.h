#pragma once
#include <d3d12.h>
#include <cstdint>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")

class ConstBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	ComPtr<ID3D12Resource> material_;

public:
	void Initialize(uint64_t resWidth);
	void SetGraphicsRootCBuffView(uint32_t rootparaIdx);
	ID3D12Resource* GetResource() { return material_.Get(); }
};

