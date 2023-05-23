#pragma once
#include <string>
#include "Vector4D.h"

struct ObjectData
{
	//	�t�@�C����
	std::string fileName;
	//	�I�u�W�F�N�g���
	Vector4D translation;
	Vector4D rotation;
	Vector4D scaling;
	//	�e���
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

