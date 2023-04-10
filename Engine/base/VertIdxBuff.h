#pragma once
#include "VertexStruct.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <iostream>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDirectX;

class VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
protected:
	//	vertex
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff;

	//	index
	bool ibExist = false;
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> indexBuff;

	D3D12_RESOURCE_DESC resDesc{};

private:
	void SetResDesc(UINT size);
protected:
	void BuffInitialize(ID3D12Device* dev, UINT sizeVB, UINT sizeIB = NULL, uint16_t* indices = nullptr, int indicesSize = NULL);
	void BuffUpdate(ID3D12GraphicsCommandList* cmdList);
public:
	VertIdxBuff() {};
	virtual ~VertIdxBuff() = default;

	virtual void SetVertices() = 0;

	//	Getter
	D3D12_RESOURCE_DESC ResDesc() { return resDesc; }
};
