#include "ObjCamera2D.h"

ObjCamera2D::ObjCamera2D()
{
	eye_ = { 0.0f,0.0f,-100.0f };
	target_ = { 0.0f,0.0f,0.0f };
	up_ = { 0.0f,1.0f,0.0f };

	matProjection_ = MyMath::OrthoLH(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, 0.1f, 1000.0f);

	MatUpdate();

	CalcDirectionVec();
}

void ObjCamera2D::Initialize(Vector3D eye, Vector3D target, Vector3D up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;

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
