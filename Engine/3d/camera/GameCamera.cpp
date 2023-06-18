#include "GameCamera.h"

void GameCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	CalcDirectionVec();
}

void GameCamera::Update()
{
	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();
}
