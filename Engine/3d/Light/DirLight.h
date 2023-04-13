#pragma once
#include "MyMath.h"
#include "ICamera.h"
#include <memory>

class DirLight
{
public:
	struct ConstBuffData
	{
		Vector3D lightv;
		float pad1;
		Vector3D lightcolor;
		unsigned int active;
	};

private:
	Vector3D center = { 0.0f, 1.0f, 0.0f };
	Vector3D lightdir = { 1,1,0 };
	Vector3D lightcolor = { 1,1,1 };
	bool active = false;

	bool shadowing = false;

	std::unique_ptr<ICamera> lightCamera;
public:
	void SetShadow(bool shadowflag);
	void SetLightDir(const Vector3D& lightdir_) {
		lightdir = lightdir_;
		lightdir.normalize();
	}
	const Vector3D& GetLightDir() { return lightdir; }
	void SetLightColor(const Vector3D& lightcolor_) { lightcolor = lightcolor_; }
	const Vector3D& GetLightColor() { return lightcolor; }
	void SetActive(bool active_) { active = active_; }
	bool IsActive() { return active; }
	ICamera* GetCamera() { return lightCamera.get(); }
};

