#pragma once
#include "MyMath.h"

class DistanceFog
{
private:
	bool active_ = false;
	Vector3D color_ = Vector3D(1.0f, 1.0f, 1.0f);
	float start_ = 0.1f;
	float end_ = 5.0f;
	float fogFar_ = 30.0f;
	float fogNear_ = 0.1f;

public:

	//	Getter
	bool GetIsActive() { return active_; }
	const Vector3D& GetColor() { return color_; }
	float GetStart() { return start_; }
	float GetEnd() { return end_; }
	float GetFar() { return fogFar_; }
	float GetNear() { return fogNear_; }

	//	Setter
	void SetActive(bool active) { active_ = active; }
	void SetColor(const Vector3D& color) { color_ = color; }
	void SetStart(float start) { start_ = start; }
	void SetEnd(float end) { end_ = end; }
	void SetFar(float fogFar) { fogFar_ = fogFar; }
	void SetNear(float fogNear) { fogNear_ = fogNear; }
};

