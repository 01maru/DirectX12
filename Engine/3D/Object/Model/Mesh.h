#pragma once
#include "VertIdxBuff.h"
#include <iostream>
#include <unordered_map>

class Material;

class Mesh :public VertIdxBuff
{
private:
    std::vector<ModelVertex> vertices_;        // 頂点データ配列
    std::vector<uint16_t> indices_;          // インデックス配列
    std::string diffuseMap_;                 // テクスチャのファイルパス
    Material* mtl_ = nullptr;
    std::unordered_map<uint16_t, std::vector<uint16_t>> smoothData_;

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
    inline const std::vector<uint16_t>& GetIndices() { return indices_; }

    //  Setter
    void SetMaterial(Material* material) { mtl_ = material; }
    void SetBone(size_t vertexID, uint16_t boneIndex, float weight);
    void SetTextureFilePath(const std::string& filePath);

    //  Add
    void AddIndex(uint16_t index) { indices_.emplace_back(index); }
    void AddVertex(const ModelVertex& vertex) { vertices_.emplace_back(vertex); }
    void AddSmoothData(uint16_t indexPosition, uint16_t indexVertex) { smoothData_[indexPosition].emplace_back(indexVertex); }
};

