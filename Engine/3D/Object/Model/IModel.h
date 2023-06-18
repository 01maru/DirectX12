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
	static const int sMAX_BONE_INDICES = 4;

	//	メッシュ
	std::vector<std::unique_ptr<Mesh>> meshes_;
	//	マテリアル
	std::unordered_map<std::string, std::unique_ptr<Material>> materials_;

	//	boneの情報
	Matrix globalInverseTransform_;
	std::map<std::string, size_t> boneMapping_;
	size_t numBones_ = 0;
	std::vector<BoneInfo> boneInfo_;

protected:
	virtual void LoadModel(const std::string& modelname, bool smoothing) = 0;

public:
	virtual ~IModel() = default;
	//	ファイル名とスムージングするか
	void Initialize(const char* filename, bool smoothing = false);
	void Draw();
	void DrawShadowReciever();

	virtual void BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms) = 0;
	void AddMaterial(Material* material) { materials_.emplace(material->name_, material); }

	//　Getter
	//const std::vector<Mesh*>& GetMeshes() { return meshes_; }
	const Matrix& GetModelTransform() { return globalInverseTransform_; }
	size_t GetNumBones() { return numBones_; }
	std::vector<BoneInfo> GetBoneInfo() { return boneInfo_; }
};

