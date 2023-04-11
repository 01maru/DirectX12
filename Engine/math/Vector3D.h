#pragma once

#pragma region 前置宣言
class Matrix;
class Vector2D;
#pragma endregion

class Vector3D
{
public:
	float x;
	float y;
	float z;
public:
	//	コンストラクタ
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector2D& vec, float z);

	//	ベクトルの長さ
	float length() const;
	//	正規化
	Vector3D& normalize();
	//	内積
	float dot(const Vector3D& v) const;
	//	外積
	Vector3D cross(const Vector3D& v) const;

	Vector3D operator+() const;
	Vector3D operator-() const;
	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);
	Vector3D& operator/=(float s);
	Vector3D& operator*=(float s);
};

const Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
const Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
const Vector3D operator*(const Vector3D& v, float s);
const Vector3D operator*(float s, const Vector3D& v);

const Vector3D Vec3Transform(const Vector3D& v, const Matrix& m);
const Vector3D Vec3TransformNormal(const Vector3D& v, const Matrix& m);
//頂点ABCで作られたポリゴンから法線を計算する。
Vector3D CreatePolygonNormal(Vector3D a, Vector3D b, Vector3D c);