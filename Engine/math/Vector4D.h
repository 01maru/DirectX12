#pragma once

class Vector3D;
class Matrix;

class Vector4D
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	//	�R���X�g���N�^
	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(const Vector3D& v, float w);
	//	�x�N�g���̒���
	float GetLength() const;
	//	���K��
	Vector4D& GetNormalize();
	void Normalize();

	Vector4D operator+() const;
	Vector4D operator-() const;
	Vector4D& operator-=(const Vector4D& v);
	Vector4D& operator/=(float s);
	Vector4D& operator*=(const Matrix& m);
};

const Vector4D operator-(const Vector4D& v1, const Vector4D& v2);