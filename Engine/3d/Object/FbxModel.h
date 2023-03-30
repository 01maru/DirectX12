#pragma once
#include "IModel.h"
#include <assimp/Importer.hpp>

#pragma region Assimp
struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiScene;
#pragma endregion

class FbxModel :public IModel
{
private:
	//	fbx‚Ìî•ñæ“¾—p•Ï”
	const aiScene* modelScene;
	Assimp::Importer importer;
public:
	FbxModel(const char* filename, bool smoothing = false);
	~FbxModel() override;
	void BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms) override;
protected:
	void LoadModel(const std::string& modelname, bool smoothing) override;
private:

	void LoadMaterial(Mesh* dst, const aiMaterial* src, int index);
	void LoadMesh(Mesh& dst, const aiMesh* src);
	void LoadBone(UINT meshIndex, const aiMesh* src);
	void LoadTexture(const std::string& filename, Mesh& dst, const aiMaterial* src);

	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform);
};

