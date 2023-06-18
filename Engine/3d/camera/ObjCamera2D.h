#pragma once
#include "ICamera.h"

class ObjCamera2D :public ICamera
{
public:
	ObjCamera2D();
	~ObjCamera2D() override {};

	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

