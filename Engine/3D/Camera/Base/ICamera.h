#pragma once
#include "MyMath.h"
#include "Window.h"

class ICamera
{
protected:
	Matrix matView;
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(48.0f), 0.1f, 1000.0f);

	Vector3D eye;		//	���_���W
	Vector3D target;	//	�����_���W
	Vector3D up;		//	������x�N�g��
	Vector3D frontVec;
	Vector3D rightVec;
	Vector3D downVec;

	Matrix billboard;
	Matrix billboardY;

	//	�r���{�[�h�v�Z�p(���_�⒍���_�̒l���ς����������x�N�g���̍X�V���Ă���v�Z����K�v����)
	void CalcBillboard();
	//	�����x�N�g���v�Z
	void CalcDirectionVec();
public:
	virtual ~ICamera() = default;
	virtual void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) = 0;
	virtual void Update() = 0;
	//	�r���[�s��X�V
	void MatUpdate() { matView = MyMath::LookAtLH(eye, target, up); }

	//	Getter
	const Vector3D& GetEye() { return eye; }
	const Vector3D& GetTarget() { return target; }
	const Vector3D& GetUp() { return up; }
	const Vector3D& GetFrontVec() { return frontVec; }
	const Vector3D& GetRightVec() { return rightVec; }
	const Vector3D& GetDownVec() { return downVec; }
	const Matrix& GetView() { return matView; }
	const Matrix& GetProjection() { return matProjection; }
	Matrix GetViewProj() const { return matView * matProjection; }
	const Matrix& GetBillboard() { return billboard; }
	const Matrix& GetBillboardY() { return billboardY; }

	//	Setter
	//	fovY�̒P�ʂ̓��W�A��
	void SetProjectionMatrix(int width, int height, float fovY);
	void SetTarget(const Vector3D& t) { target = t; }
	void SetEye(const Vector3D& e) { eye = e; }
	void EyeMove(const Vector3D& moveE) { eye += moveE; }
};

