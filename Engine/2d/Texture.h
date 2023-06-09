#pragma once
#include <d3d12.h>
#include <string>
#include <wrl.h>
#pragma comment(lib, "d3d12.lib")

class Texture
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::string name;
	//	�����l��-1
	int handle = -1;
	ComPtr<ID3D12Resource> texBuff;
public:
	void Initialize(const std::string& texName, int handle_, ID3D12Resource* texBuff_ptr);
	void CreateNoTexture(const std::string& texName);

	int GetHandle() { return handle; }
	ID3D12Resource* GetResourceBuff() { return texBuff.Get(); }
	ID3D12Resource** GetResourceBuffAddress() { return texBuff.ReleaseAndGetAddressOf(); }
};

