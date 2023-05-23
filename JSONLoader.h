#pragma once
#include <string>
#include "Vector3D.h"
#include "IModel.h"
#include "Object3D.h"

struct ObjectData
{
	//	�t�@�C����
	std::string fileName;
	//	�I�u�W�F�N�g���
	Vector3D translation;
	Vector3D rotation;
	Vector3D scaling;
	//	�e���
	ObjectData* parent = nullptr;
};

struct LevelData
{
	std::vector<ObjectData> objects;
};

class JSONLoader
{
private:
	//	���f���f�[�^�R���e�i
	std::map<std::string, IModel*> models;
	//	�I�u�W�F�N�g�f�[�^
	std::vector<Object3D*> objects;
	LevelData* levelData = nullptr;

public:
	void LoadJSON(std::string jsonname);
	void LoadModel();
	void MatUpdate();
	void Draw();
};

