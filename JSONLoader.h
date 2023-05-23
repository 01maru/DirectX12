#pragma once
#include <string>
#include "Vector3D.h"
#include "IModel.h"
#include "Object3D.h"

struct ObjectData
{
	//	ファイル名
	std::string fileName;
	//	オブジェクト情報
	Vector3D translation;
	Vector3D rotation;
	Vector3D scaling;
	//	親情報
	ObjectData* parent = nullptr;
};

struct LevelData
{
	std::vector<ObjectData> objects;
};

class JSONLoader
{
private:
	//	モデルデータコンテナ
	std::map<std::string, IModel*> models;
	//	オブジェクトデータ
	std::vector<Object3D*> objects;
	LevelData* levelData = nullptr;

public:
	void LoadJSON(std::string jsonname);
	void LoadModel();
	void MatUpdate();
	void Draw();
};

