#pragma once
#include "MyMath.h"
#include "ICamera.h"
#include <memory>

class DirLight
{
private:
	bool active_ = false;
	Vector3D dir_ = { 0,1,0 };
	Vector3D color_ = { 1,1,1 };

	bool shadowing_ = false;
	Vector3D center_ = { 0.0f, 1.0f, 0.0f };

	std::unique_ptr<ICamera> lightCamera;
public:
	//	Getter
	bool GetIsActive() { return active_; }
	const Vector3D& GetLightDir() { return dir_; }
	const Vector3D& GetLightColor() { return color_; }
	ICamera* GetCamera() { return lightCamera.get(); }

	//	Setter
	void SetActive(bool active) { active_ = active; }
	void SetLightDir(const Vector3D& dir);
	void SetLightColor(const Vector3D& color) { color_ = color; }
	void SetShadow(bool shadowflag);
};

