#include "VertIdxBuff.h"
#include "DirectX.h"
#include <cassert>

void VertIdxBuff::SetResDesc(UINT size)
{
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = size; // 頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;		//	アンチエイリアシング用のパラメータ
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void VertIdxBuff::Initialize(uint32_t sizeVB, const std::vector<uint16_t>& indices)
{
	Initialize(sizeVB);

#pragma region IB
	if (indices.size() != 0) {
		HRESULT result;
		ID3D12Device* dev = MyDirectX::GetInstance()->GetDev();
		D3D12_HEAP_PROPERTIES heapProp{};

		//	ヒープの設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用(CPUからアクセスできる)		

		ibActive_ = true;

		uint16_t sizeIB = static_cast<uint16_t>(sizeof(uint16_t) * indices.size());
		SetResDesc(sizeIB);
		result = dev->CreateCommittedResource(
			&heapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc_, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff_));
		assert(SUCCEEDED(result));
		//	インデックスバッファマッピング
		SetIndices(indices);
		//	インデックスバッファビュー作成
		ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
		ibView_.Format = DXGI_FORMAT_R16_UINT;
		ibView_.SizeInBytes = sizeIB;
	}
#pragma endregion
}

void VertIdxBuff::Initialize(uint32_t sizeVB)
{
	ID3D12Device* dev = MyDirectX::GetInstance()->GetDev();
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
		&resDesc_,							// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成(GPUで利用するため)
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;

	SetVertices();
#pragma endregion
}

void VertIdxBuff::SetIndices(const std::vector<uint16_t>& indices)
{
	//	インデックスバッファマッピング
	uint16_t* indexMap = nullptr;
	HRESULT result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i]; // 座標をコピー
	}
	// 繋がりを解除
	indexBuff_->Unmap(0, nullptr);
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
		&resDesc_,							// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成(GPUで利用するため)
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;

	SetVertices();
#pragma endregion

#pragma region IB
	if (indices != nullptr) {
		ibActive_ = true;

		SetResDesc(sizeIB);
		result = dev->CreateCommittedResource(
			&heapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc_, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff_));
		assert(SUCCEEDED(result));
		//	インデックスバッファマッピング
		uint16_t* indexMap = nullptr;
		result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));
		// 全頂点に対して
		for (int i = 0; i < indicesSize; i++) {
			indexMap[i] = indices[i]; // 座標をコピー
		}
		// 繋がりを解除
		indexBuff_->Unmap(0, nullptr);
		//	インデックスバッファビュー作成
		ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
		ibView_.Format = DXGI_FORMAT_R16_UINT;
		ibView_.SizeInBytes = sizeIB;
	}
#pragma endregion
}

void VertIdxBuff::IASetVertIdxBuff()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	//	インデックスバッファビュー設定コマンド
	if (ibActive_) {
		cmdList->IASetIndexBuffer(&ibView_);
	}
}
