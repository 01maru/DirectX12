#include "ICamera.h"

void ICamera::CalcBillboard()
{
	//	行列初期化
	billboard_.Identity();

#pragma region 全方向ビルボード
	billboard_.m[0][0] = rightVec_.x;
	billboard_.m[0][1] = rightVec_.y;
	billboard_.m[0][2] = rightVec_.z;
	billboard_.m[1][0] = -downVec_.x;
	billboard_.m[1][1] = -downVec_.y;
	billboard_.m[1][2] = -downVec_.z;
	billboard_.m[2][0] = frontVec_.x;
	billboard_.m[2][1] = frontVec_.y;
	billboard_.m[2][2] = frontVec_.z;
#pragma endregion

	//	行列初期化
	billboardY_.Identity();

#pragma region Y軸ビルボード
	billboardY_.m[0][0] = rightVec_.x;
	billboardY_.m[0][1] = rightVec_.y;
	billboardY_.m[0][2] = rightVec_.z;
	Vector3D billYvecZ = rightVec_.cross(up_);
	billboardY_.m[2][0] = billYvecZ.x;
	billboardY_.m[2][1] = billYvecZ.y;
	billboardY_.m[2][2] = billYvecZ.z;
#pragma endregion
}

void ICamera::CalcDirectionVec()
{
	//	前方方向ベクトル
	frontVec_ = target_ - eye_;
	frontVec_.Normalize();

	//	右方向ベクトル
	rightVec_ = Vector3D(0, 1, 0).cross(frontVec_);
	if (rightVec_ == Vector3D()) {
		rightVec_ = Vector3D(1.0f, 0.0f, 0.0f);
	}
	//	下方向ベクトル
	downVec_ = rightVec_.cross(frontVec_);

	//	ベクトルの正規化
	rightVec_.Normalize();
	downVec_.Normalize();
}

void ICamera::SetProjectionMatrix(int32_t width, int32_t height, float fovY)
{
	matProjection_ = MyMath::PerspectiveFovLH(width, height, fovY, 0.1f, 1000.0f);
}
