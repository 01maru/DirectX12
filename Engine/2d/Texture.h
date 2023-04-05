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
	//	‰Šú’l‚Í-1
	int handle = -1;
	ID3D12Resource* texBuff = nullptr;
public:
	void Create(const std::string& texName, int handle_, ID3D12Resource* texBuff_ptr);

	int GetHandle() { return handle; }
	ID3D12Resource* GetResourceBuff() { return texBuff; }
};

