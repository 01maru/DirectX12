#pragma once
#include "ICamera.h"

class ObjCamera2D :public ICamera
{
public:
	ObjCamera2D();
	~ObjCamera2D() override {};

	void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) override;
	void Update() override;
};

