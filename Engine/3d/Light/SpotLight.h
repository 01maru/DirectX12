#pragma once
#include "MyMath.h"

class SpotLight
{
private:
	bool active_ = false;
	Vector3D dir_ = { 1,0,0 };
	Vector3D pos_ = { 0,0,0 };
	Vector3D color_ = { 1,1,1 };
	Vector3D atten_ = { 1,1,1 };
	Vector2D factorAngleCos_ = { 0.5f,0.2f };

public:

	//	Getter
	bool GetIsActive() { return active_; }
	const Vector3D& GetLightDir() { return dir_; }
	const Vector3D& GetLightPos() { return pos_; }
	const Vector3D& GetLightColor() { return color_; }
	const Vector3D& GetLightAtten() { return atten_; }
	const Vector2D& GetLightFactorAngleCos() { return factorAngleCos_; }

	//	Setter
	void SetActive(bool active) { active_ = active; }
	void SetLightDir(const Vector3D& dir);
	void SetLightPos(const Vector3D& pos) { pos_ = pos; }
	void SetLightColor(const Vector3D& color) { color_ = color; }
	void SetLightAtten(const Vector3D& atten) { atten_ = atten; }
	void SetLightFactorAngle(const Vector2D& factorAngle);
};

