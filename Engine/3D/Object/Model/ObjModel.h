#pragma once
#include "IModel.h"

class ObjModel :public IModel
{
protected:
	void LoadModel(const std::string& modelname, bool smoothing) override;

private:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void AddIndices(const std::vector<uint16_t>& indices, Mesh* mesh);
public:
	ObjModel(const char* filename, bool smoothing = false);
	~ObjModel() override;
	void BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms) override;
};

