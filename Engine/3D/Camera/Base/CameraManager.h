#pragma once
#include "ICamera.h"
#include <vector>

class CameraManager
{
private:
	std::vector<ICamera> cameras;

	CameraManager() {};
	~CameraManager() {};
public:
	static CameraManager* GetInstance();
	CameraManager(const CameraManager& obj) = delete;
	CameraManager& operator=(const CameraManager& obj) = delete;

	void Update();
};

