#pragma once

class Vector2D
{
public:
	float x;
	float y;
public:
	//	�R���X�g���N�^
	Vector2D();
	Vector2D(float x, float y);

	//	�x�N�g���̒���
	float length() const;
	//	���K��
	Vector2D& normalize();

	Vector2D operator+() const;
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator/=(float s);
	Vector2D& operator*=(float s);
	bool operator!=(const Vector2D& vec);
};

const Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
const Vector2D operator*(const Vector2D& v, float s);
const Vector2D operator*(float s, const Vector2D& v);
