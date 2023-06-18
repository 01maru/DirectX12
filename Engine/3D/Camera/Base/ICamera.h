#pragma once
#include "MyMath.h"
#include "Window.h"

class ICamera
{
protected:
	Matrix matView_;
	Matrix matProjection_ = MyMath::PerspectiveFovLH(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, MyMath::ConvertToRad(48.0f), 0.1f, 1000.0f);

	Vector3D eye_;		//	視点座標
	Vector3D target_;	//	注視点座標
	Vector3D up_;		//	上方向ベクトル
	Vector3D frontVec_;
	Vector3D rightVec_;
	Vector3D downVec_;

	Matrix billboard_;
	Matrix billboardY_;

protected:
	//	ビルボード計算用(視点や注視点の値が変わったら方向ベクトルの更新してから計算する必要あり)
	void CalcBillboard();
	//	方向ベクトル計算
	void CalcDirectionVec();

public:
	virtual ~ICamera() = default;
	virtual void Initialize(Vector3D eye, Vector3D target, Vector3D up) = 0;
	virtual void Update() = 0;
	//	ビュー行列更新
	void MatUpdate() { matView_ = MyMath::LookAtLH(eye_, target_, up_); }

	//	Getter
	const Vector3D& GetEye() { return eye_; }
	const Vector3D& GetTarget() { return target_; }
	const Vector3D& GetUp() { return up_; }
	const Vector3D& GetFrontVec() { return frontVec_; }
	const Vector3D& GetRightVec() { return rightVec_; }
	const Vector3D& GetDownVec() { return downVec_; }
	const Matrix& GetView() { return matView_; }
	const Matrix& GetProjection() { return matProjection_; }
	Matrix GetViewProj() const { return matView_ * matProjection_; }
	const Matrix& GetBillboard() { return billboard_; }
	const Matrix& GetBillboardY() { return billboardY_; }

	//	Setter
	//	fovYの単位はラジアン
	void SetProjectionMatrix(int32_t width, int32_t height, float fovY);
	void SetTarget(const Vector3D& t) { target_ = t; }
	void SetEye(const Vector3D& e) { eye_ = e; }
	void EyeMove(const Vector3D& moveE) { eye_ += moveE; }
};

