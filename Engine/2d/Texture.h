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
	//	èâä˙ílÇÕ-1
	int handle = -1;
	ID3D12Resource* texBuff = nullptr;
public:
	void Initialize(const std::string& texName, int handle_, ID3D12Resource* texBuff_ptr);
	void CreateNoTexture(const std::string& texName);

	int GetHandle() { return handle; }
	ID3D12Resource* GetResourceBuff() { return texBuff; }
	ID3D12Resource** GetResourceBuffPtrPtr() { return &texBuff; }
};

