#pragma once
#include "VertexStruct.h"
#include <wrl.h>
#include <d3d12.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")

class VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:
	//	vertex
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	//	index
	bool ibActive_ = false;
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	ComPtr<ID3D12Resource> indexBuff_;

	D3D12_RESOURCE_DESC resDesc_{};

private:
	void SetResDesc(UINT size);

protected:
	void Initialize(uint32_t sizeVB, const std::vector<uint32_t>& indices);
	void Initialize(uint32_t sizeVB);
	void BuffInitialize(ID3D12Device* dev, UINT sizeVB, UINT sizeIB = NULL, uint16_t* indices = nullptr, int indicesSize = NULL);
	void IASetVertIdxBuff();

public:
	VertIdxBuff() {};
	virtual ~VertIdxBuff() = default;

	virtual void SetVertices() = 0;
	void SetIndices(const std::vector<uint32_t>& indices);

	//	Getter
	const D3D12_RESOURCE_DESC& GetResDesc() { return resDesc_; }
};
