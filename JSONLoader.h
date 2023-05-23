#pragma once
#include <string>
#include "Vector4D.h"

struct ObjectData
{
	//	ファイル名
	std::string fileName;
	//	オブジェクト情報
	Vector4D translation;
	Vector4D rotation;
	Vector4D scaling;
	//	親情報
	ObjectData* parent = nullptr;
};

struct LevelData
{
	std::list<ObjectData> objects;
};

class JSONLoader
{
public:
	LevelData* LoadJSON(std::string jsonname);
};

