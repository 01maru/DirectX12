#include "VertIdxBuff.h"
#include "DirectX.h"
#include <wrl.h>
#include <vector>
#include <cassert>

void VertIdxBuff::BuffUpdate(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//	インデックスバッファビュー設定コマンド
	if (ibExist) {
		cmdList->IASetIndexBuffer(&ibView);
	}
}

void VertIdxBuff::BuffInitialize(ID3D12Device* dev, UINT sizeVB, UINT sizeIB, uint16_t* indices, int indicesSize)
{
	D3D12_HEAP_PROPERTIES heapProp{};

	//	ヒープの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用(CPUからアクセスできる)

#pragma region VB
	// リソース設定
	SetResDesc(sizeVB);

	//	GPU側にメモリ確保
	HRESULT result = dev->CreateCommittedResource(
		&heapProp,							// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,							// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成(GPUで利用するため)
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;

	SetVertices();
#pragma endregion

#pragma region IB
	if (indices != nullptr) {
		ibExist = true;

		SetResDesc(sizeIB);
		result = dev->CreateCommittedResource(
			&heapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));
		assert(SUCCEEDED(result));
		//	インデックスバッファマッピング
		uint16_t* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));
		// 全頂点に対して
		for (int i = 0; i < indicesSize; i++) {
			indexMap[i] = indices[i]; // 座標をコピー
		}
		// 繋がりを解除
		indexBuff->Unmap(0, nullptr);
		//	インデックスバッファビュー作成
		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;
	}
#pragma endregion
}

void VertIdxBuff::SetResDesc(UINT size)
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = size; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;		//	アンチエイリアシング用のパラメータ
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}
