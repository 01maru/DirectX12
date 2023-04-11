#pragma once
#include "MyMath.h"
#include "Window.h"

class ICamera
{
protected:
	Matrix matView;
	Vector3D eye;		//	���_���W
	Vector3D target;	//	�����_���W
	Vector3D up;		//	������x�N�g��
	Vector3D frontVec;
	Vector3D rightVec;
	Vector3D downVec;

	Matrix billboard;
	Matrix billboardY;
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(48.0f), 0.1f, 1000.0f);

	//	�r���{�[�h�v�Z�p
	void CalcBillboard();
	void CalcDirectionVec();
public:
	virtual ~ICamera() = default;
	virtual void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) = 0;
	virtual void Update() = 0;
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
	const Matrix& GetBillboard() { return billboard; }
	const Matrix& GetBillboardY() { return billboardY; }
	Matrix GetViewProj() const { 
		return matView * matProjection;
	}
	//	Setter
	//	fovY�̒P�ʂ̓��W�A��
	void SetProjectionMatrix(int width, int height, float fovY);
	void SetTarget(Vector3D t) { target = t; }
	void SetEye(Vector3D e) { eye = e; }
	void EyeMove(Vector3D moveE) { eye += moveE; }
};

