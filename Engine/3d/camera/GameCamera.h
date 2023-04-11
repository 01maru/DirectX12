#pragma once
#include "ICamera.h"

class GameCamera :public ICamera
{
private:

public:
	GameCamera() {};
	~GameCamera() override;

	void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) override;
	void Update() override;
};

