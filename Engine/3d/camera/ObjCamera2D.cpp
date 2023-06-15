#include "ObjCamera2D.h"

ObjCamera2D::ObjCamera2D()
{
	eye = { 0.0f,0.0f,-100.0f };
	target = { 0.0f,0.0f,0.0f };
	up = { 0.0f,1.0f,0.0f };

	matProjection = MyMath::OrthoLH(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, 0.1f, 1000.0f);

	MatUpdate();

	CalcDirectionVec();
}

void ObjCamera2D::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	eye = eye_;
	target = target_;
	up = up_;

	MatUpdate();

	CalcDirectionVec();
}

void ObjCamera2D::Update()
{
	CalcDirectionVec();

#pragma region ビルボード
	CalcBillboard();
#pragma endregion

	MatUpdate();
}
