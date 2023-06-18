#pragma once
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <cstdint>

#pragma comment(lib, "d3d12.lib")

class Texture
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::string name_;
	//	初期値は-1
	int32_t handle_ = -1;
	ComPtr<ID3D12Resource> texBuff_;
public:
	void Initialize(const std::string& texName, int32_t handle, ID3D12Resource* texBuff_ptr);
	void CreateNoTexture(const std::string& texName);

	int GetHandle() { return handle_; }
	ID3D12Resource* GetResourceBuff() { return texBuff_.Get(); }
	ID3D12Resource** GetResourceBuffAddress() { return texBuff_.ReleaseAndGetAddressOf(); }
};

