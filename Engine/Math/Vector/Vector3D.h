#pragma once

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
	Vector3D(int x, int y, int z);
	Vector3D(const Vector2D& vec, float z);
	Vector3D(const Vector2D& vec, int z);
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

const Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
const Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
const Vector3D operator*(const Vector3D& v, float s);
const Vector3D operator*(float s, const Vector3D& v);

const Vector3D Vec3Transform(const Vector3D& v, const Matrix& m);
const Vector3D Vec3TransformNormal(const Vector3D& v, const Matrix& m);
