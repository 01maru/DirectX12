#include "Mesh.h"
#include "DirectX.h"
#include "TextureManager.h"
#include <cassert>

void Mesh::Initialzie()
{
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	BuffInitialize(MyDirectX::GetInstance()->GetDev(), sizeVB, sizeIB, &indices_.front(), (int)indices_.size());
	//VertIdxBuff::Initialize(sizeVB, indices);
}

void Mesh::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	VertIdxBuff::IASetVertIdxBuff();

	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(mtl_->GetTextureHandle()));

	mtl_->SetGraphicsRootCBuffView(1);

	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Mesh::DrawShadowReciever()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	VertIdxBuff::IASetVertIdxBuff();

	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(mtl_->GetTextureHandle()));

	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Mesh::CalcSmoothedNormals()
{
	for (auto itr = smoothData_.begin(); itr != smoothData_.end(); ++itr) {
		std::vector<unsigned short>& v = itr->second;

		Vector3D normal;
		for (unsigned short index : v) {
			normal += vertices_[index].normal;
		}
		normal /= (float)v.size();
		normal.Normalize();

		for (unsigned short index : v) {
			vertices_[index].normal = normal;
		}
	}
}

void Mesh::SetBone(int vertexID, UINT boneIndex, float weight)
{
	for (UINT i = 0; i < 4; i++) {
		//	�{�[����weight��0��������l���
		if (vertices_[vertexID].boneWeight[i] == 0.0f) {
			vertices_[vertexID].boneIndex[i] = boneIndex;
			vertices_[vertexID].boneWeight[i] = weight;
			return;
		}
	}
}

void Mesh::SetTextureFilePath(const std::string& filePath)
{
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, mtl_->wfilepath_, _countof(mtl_->wfilepath_));
}

void Mesh::SetVertices()
{
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	ModelVertex* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);
	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}
