#include "NormalCamera.h"

NormalCamera::~NormalCamera()
{
}

void NormalCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	eye = eye_;
	target = target_;
	up = up_;
	//matProjection = MyMath::OrthoLH(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, 0.1f, 50.0f);
	MatUpdate();

	CalcDirectionVec();

	up = -downVec;

	MatUpdate();
}

void NormalCamera::Update()
{
	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();
}
