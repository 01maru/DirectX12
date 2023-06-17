#include "ConstBuff.h"
#include "DirectX.h"
#include <cassert>

void ConstBuff::Initialize(uint64_t resWidth)
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};

	//	ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = (resWidth + 0xFF) & ~0xFF;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	生成
	result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_));

	assert(SUCCEEDED(result));
}

void ConstBuff::SetGraphicsRootCBuffView(uint32_t rootparaIdx)
{
	//	定数バッファビュー(CBV)の設定コマンド
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(rootparaIdx, material_->GetGPUVirtualAddress());
}
