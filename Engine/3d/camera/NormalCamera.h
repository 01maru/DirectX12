#pragma once
#include "ICamera.h"

class NormalCamera :public ICamera
{
public:
	NormalCamera() {};
	~NormalCamera() override {};

	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};
