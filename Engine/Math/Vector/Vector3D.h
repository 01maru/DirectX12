#pragma once
#include <cstdint>

class Matrix;
class Vector2D;
class Vector4D;

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
	Vector3D(int32_t x, int32_t y, int32_t z);
	Vector3D(const Vector2D& vec, float z);
	Vector3D(const Vector2D& vec, int32_t z);
	Vector3D(const Vector4D& vec);

	//	ベクトルの長さ
	float GetLength() const;
	//	正規化
	void Normalize();
	Vector3D& GetNormalize();
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

	bool operator==(const Vector3D& vec);
	bool operator!=(const Vector3D& vec);
};

Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
Vector3D operator*(const Vector3D& v, float s);
Vector3D operator*(float s, const Vector3D& v);

Vector3D Vec3Transform(const Vector3D& v, const Matrix& m);
Vector3D Vec3TransformNormal(const Vector3D& v, const Matrix& m);
