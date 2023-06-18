#pragma once
#include "Vector2D.h"
#include "ICamera.h"

class MyDebugCamera :public ICamera
{
private:
	float disEyeTarget_ = 0.0f;

	Vector2D cursorPos_;

	enum MoveMode {
		NoMove,
		TranslationMove,
		RotationMove,
	};
	MoveMode mode_ = NoMove;
public:
	MyDebugCamera() {};
	~MyDebugCamera() override {};

	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

