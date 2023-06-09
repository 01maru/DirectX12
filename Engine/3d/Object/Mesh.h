#pragma once
#include "VertIdxBuff.h"
#include "Material.h"
#include "DirectX.h"
#include <unordered_map>

class Mesh :public VertIdxBuff
{
private:
    static MyDirectX* dx;
private:
    std::vector<FBXVertex> vertices; // 頂点データの配列
    std::vector<uint16_t> indices; // インデックスの配列
    Material* mtl = nullptr;
    std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
    std::string diffuseMap; // テクスチャのファイルパス

public:
    void CreateBuff();
    void Draw();
    void DrawShadowReciever();
    void CalcSmoothedNormals();

    void SetMaterial(Material* material) { mtl = material; }

    Material* GetMaterial() { return mtl; }
    size_t GetVertexCount() { return vertices.size(); }

    inline const std::vector<FBXVertex>& GetVertices() { return vertices; }
    inline const std::vector<unsigned short>& GetIndices() { return indices; }

    void AddIndex(unsigned short index) { indices.emplace_back(index); }
    void AddVertex(const FBXVertex& vertex) { vertices.emplace_back(vertex); }
    void SetBone(int vertexID, UINT boneIndex, float weight);
    void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) { smoothData[indexPosition].emplace_back(indexVertex); }
    void SetTextureFilePath(const std::string& filePath);
private:
    void SetVertices() override;
};

