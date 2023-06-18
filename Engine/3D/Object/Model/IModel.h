#pragma once
#include "Material.h"
#include "Mesh.h"
#include <map>

struct BoneInfo
{
	Matrix boneOffset;
	Matrix finalTransformation;
};

class IModel
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//	boneの最大数
	static const int MAX_BONE_INDICES = 4;

	//	メッシュ
	std::vector<std::unique_ptr<Mesh>> meshes;
	//	マテリアル
	std::unordered_map<std::string, std::unique_ptr<Material>> materials;

	//	boneの情報
	Matrix globalInverseTransform;
	std::map<std::string, UINT> boneMapping;
	UINT numBones = 0;
	std::vector<BoneInfo> boneInfo;

protected:
	virtual void LoadModel(const std::string& modelname, bool smoothing) = 0;

public:
	virtual ~IModel() = default;
	//	ファイル名とスムージングするか
	void Initialize(const char* filename, bool smoothing = false);
	void Draw();
	void DrawShadowReciever();

	virtual void BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms) = 0;
	void AddMaterial(Material* material) { materials.emplace(material->name_, material); }

	//　Getter
	//const std::vector<Mesh*>& GetMeshes() { return meshes; }
	const Matrix& GetModelTransform() { return globalInverseTransform; }
	UINT GetNumBones() { return numBones; }
	std::vector<BoneInfo> GetBoneInfo() { return boneInfo; }
};

