#pragma once

#pragma region 前置宣言
class Vector3D;
class Matrix;
#pragma endregion

class Vector4D
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	//	コンストラクタ
	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(Vector3D v3d, float w);
	//	ベクトルの長さ
	float length() const;
	//	正規化
	Vector4D& normalize();

	Vector4D operator+() const;
	Vector4D operator-() const;
	Vector4D& operator-=(const Vector4D& v);
	Vector4D& operator/=(float s);
	Vector4D& operator*=(const Matrix& m);
};

const Vector4D operator-(const Vector4D& v1, const Vector4D& v2);