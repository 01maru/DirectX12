#pragma once

class Vector2D
{
public:
	float x;
	float y;
public:
	//	�R���X�g���N�^
	Vector2D() :x(0), y(0) {}
	Vector2D(float x, float y) :x(x), y(y) {}
	Vector2D(int x, int y) :x((float)x), y((float)y) {}

	//	�x�N�g���̒���
	float GetLength() const;
	//	�x�N�g���̐��K��
	void Normalize();
	//	���K�������x�N�g����return
	Vector2D& GetNormalize();

	Vector2D operator+() const;
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator/=(float s);
	Vector2D& operator*=(float s);

	bool operator!=(const Vector2D& vec);
	bool operator==(const Vector2D& vec);
};

const Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
const Vector2D operator*(const Vector2D& v, float s);
const Vector2D operator*(float s, const Vector2D& v);
