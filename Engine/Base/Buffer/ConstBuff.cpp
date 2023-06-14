#include "ConstBuff.h"
#include "DirectX.h"
#include <cassert>

void ConstBuff::Initialize(uint64_t resWidth)
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};

	//	�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = (resWidth + 0xFF) & ~0xFF;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	����
	result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
}

void ConstBuff::SetGraphicsRootCBuffView(uint32_t rootparaIdx)
{
	//	�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(rootparaIdx, material->GetGPUVirtualAddress());
}
