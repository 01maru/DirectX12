#include "NormalCamera.h"

NormalCamera::~NormalCamera()
{
}

void NormalCamera::Initialize(Vector3D eye, Vector3D target, Vector3D up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;
	//matProjection_ = MyMath::OrthoLH(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, 0.1f, 50.0f);
	MatUpdate();

	CalcDirectionVec();

	up_ = -downVec_;

	MatUpdate();
}

void NormalCamera::Update()
{
	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();
}
