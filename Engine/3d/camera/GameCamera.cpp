#include "GameCamera.h"

GameCamera::~GameCamera()
{
}

void GameCamera::Initialize(Vector3D eye, Vector3D target, Vector3D up)
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
