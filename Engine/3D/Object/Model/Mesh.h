#pragma once
#include "VertIdxBuff.h"
#include "Material.h"
#include <unordered_map>

class Mesh :public VertIdxBuff
{
private:
    std::vector<ModelVertex> vertices;        // 頂点データ配列
    std::vector<uint16_t> indices;          // インデックス配列
    std::string diffuseMap;                 // テクスチャのファイルパス
    Material* mtl = nullptr;
    std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

private:
    void SetVertices() override;

public:
    void Initialzie();
    void Draw();
    void DrawShadowReciever();
    void CalcSmoothedNormals();

    //  Getter
    Material* GetMaterial() { return mtl; }
    size_t GetVertexCount() { return vertices.size(); }
    inline const std::vector<ModelVertex>& GetVertices() { return vertices; }
    inline const std::vector<unsigned short>& GetIndices() { return indices; }

    //  Setter
    void SetMaterial(Material* material) { mtl = material; }
    void SetBone(int vertexID, UINT boneIndex, float weight);
    void SetTextureFilePath(const std::string& filePath);

    //  Add
    void AddIndex(unsigned short index) { indices.emplace_back(index); }
    void AddVertex(const ModelVertex& vertex) { vertices.emplace_back(vertex); }
    void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) { smoothData[indexPosition].emplace_back(indexVertex); }
};

