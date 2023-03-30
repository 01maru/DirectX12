#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "Material.h"
#include "Light.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
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

	static MyDirectX* dx;
	//	boneの最大数
	static const int MAX_BONE_INDICES = 4;

	//	メッシュ
	std::vector<Mesh*> meshes;
	//	マテリアル
	std::unordered_map<std::string, Material*> materials;

	//	boneの情報
	Matrix globalInverseTransform;
	std::map<std::string, UINT> boneMapping;
	UINT numBones = 0;
	std::vector<BoneInfo> boneInfo;
public:
	virtual ~IModel() = default;
	//	ファイル名とスムージングするか
	void Initialize(const char* filename, bool smoothing = false);
	void Draw();

	virtual void BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms) = 0;
	void AddMaterial(Material* material) { materials.emplace(material->name, material); }
protected:
	virtual void LoadModel(const std::string& modelname, bool smoothing) = 0;
public:
	//　Getter
	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }
	const Matrix& GetModelTransform() { return globalInverseTransform; }
	UINT GetNumBones() { return numBones; }
	std::vector<BoneInfo> GetBoneInfo() { return boneInfo; }
};

