#include "ICamera.h"

void ICamera::CalcBillboard()
{
	//	行列初期化
	billboard.Identity();

#pragma region 全方向ビルボード
	billboard.m[0][0] = rightVec.x;
	billboard.m[0][1] = rightVec.y;
	billboard.m[0][2] = rightVec.z;
	billboard.m[1][0] = -downVec.x;
	billboard.m[1][1] = -downVec.y;
	billboard.m[1][2] = -downVec.z;
	billboard.m[2][0] = frontVec.x;
	billboard.m[2][1] = frontVec.y;
	billboard.m[2][2] = frontVec.z;
#pragma endregion

	//	行列初期化
	billboardY.Identity();

#pragma region Y軸ビルボード
	billboardY.m[0][0] = rightVec.x;
	billboardY.m[0][1] = rightVec.y;
	billboardY.m[0][2] = rightVec.z;
	Vector3D billYvecZ = rightVec.cross(up);
	billboardY.m[2][0] = billYvecZ.x;
	billboardY.m[2][1] = billYvecZ.y;
	billboardY.m[2][2] = billYvecZ.z;
#pragma endregion
}

void ICamera::SetProjectionMatrix(int width, int height, float fovY)
{
	matProjection = MyMath::PerspectiveFovLH(width, height, fovY, 0.1f, 1000.0f);
}
