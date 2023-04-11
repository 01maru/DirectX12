#pragma once
#include "MyMath.h"
#include "ICamera.h"

class MyDebugCamera :public ICamera
{
private:
	float disEyeTarget = 0.0f;

	Vector2D moveCursor;
	Vector2D cursorSpd;

	Vector2D rotAngle;
	Vector2D prevRotAngle;
	Vector2D endRotAngle;

	enum MoveMode {
		NoMove,
		TranslationMove,
		RotationMove,
	};
	MoveMode mode = NoMove;
public:
	MyDebugCamera() {};
	~MyDebugCamera() override;

	void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) override;
	void Update() override;
};

