#include "GameCamera.h"

GameCamera::~GameCamera()
{
}

void GameCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	eye = eye_;
	target = target_;
	up = up_;

	MatUpdate();

	CalcDirectionVec();
}

void GameCamera::Update()
{
	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();
}
