#pragma once
#include "MyMath.h"

class CircleShadow
{
private:
	bool active_ = false;
	Vector3D dir_ = { 1,0,0 };
	Vector3D casterPos_ = { 0,0,0 };
	float disCasterLight_ = 100.0f;
	Vector3D atten_ = { 0.5f,0.6f,0.0f };
	Vector2D factorAngleCos_ = { 0.2f,0.5f };

public:

	//	Getter
	bool GetIsActive() { return active_; }
	const Vector3D& GetDir() { return dir_; }
	const Vector3D& GetCasterPos() { return casterPos_; }
	float GetDistanceCasterLight() { return disCasterLight_; }
	const Vector3D& GetAtten() { return atten_; }
	const Vector2D& GetFactorAngleCos() { return factorAngleCos_; }

	//	Setter
	void SetActive(bool active) { active_ = active; }
	void SetDir(const Vector3D& dir);
	void SetCasterPos(const Vector3D& casterPos) { casterPos_ = casterPos; }
	void SetDistanceCasterLight(float distanceCasterLight) { disCasterLight_ = distanceCasterLight; }
	void SetAtten(const Vector3D& atten) { atten_ = atten; }
	void SetFactorAngle(const Vector2D& factorAngle);
};

