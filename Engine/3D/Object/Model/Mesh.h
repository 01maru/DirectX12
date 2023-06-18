#pragma once
#include "VertIdxBuff.h"
#include "Material.h"
#include <iostream>
#include <unordered_map>

class Mesh :public VertIdxBuff
{
private:
    std::vector<ModelVertex> vertices_;        // ���_�f�[�^�z��
    std::vector<uint16_t> indices_;          // �C���f�b�N�X�z��
    std::string diffuseMap_;                 // �e�N�X�`���̃t�@�C���p�X
    Material* mtl_ = nullptr;
    std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;

private:
    void SetVertices() override;

public:
    void Initialzie();
    void Draw();
    void DrawShadowReciever();
    void CalcSmoothedNormals();

    //  Getter
    Material* GetMaterial() { return mtl_; }
    size_t GetVertexCount() { return vertices_.size(); }
    inline const std::vector<ModelVertex>& GetVertices() { return vertices_; }
    inline const std::vector<unsigned short>& GetIndices() { return indices_; }

    //  Setter
    void SetMaterial(Material* material) { mtl_ = material; }
    void SetBone(int vertexID, UINT boneIndex, float weight);
    void SetTextureFilePath(const std::string& filePath);

    //  Add
    void AddIndex(unsigned short index) { indices_.emplace_back(index); }
    void AddVertex(const ModelVertex& vertex) { vertices_.emplace_back(vertex); }
    void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) { smoothData_[indexPosition].emplace_back(indexVertex); }
};

