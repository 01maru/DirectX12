#pragma once
#include "MyMath.h"

class PointLight
{
private:
	bool active_ = false;
	Vector3D pos_;
	Vector3D color_ = { 1.0f,1.0f,1.0f };
	Vector3D atten_ = { 1.0f,1.0f,1.0f };

public:
	//	Getter
	bool GetIsActive() { return active_; }
	const Vector3D& GetLightPos() { return pos_; }
	const Vector3D& GetLightColor() { return color_; }
	const Vector3D& GetLightAtten() { return atten_; }

	//	Setter
	void SetActive(bool active) { active_ = active; }
	void SetLightPos(const Vector3D& pos) { pos_ = pos; }
	void SetLightColor(const Vector3D& color) { color_ = color; }
	void SetLightAtten(const Vector3D& atten) { atten_ = atten; }
};

