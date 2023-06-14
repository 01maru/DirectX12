#pragma once

class Vector2D
{
public:
	float x;
	float y;
public:
	//	コンストラクタ
	Vector2D() :x(0), y(0) {}
	Vector2D(float x, float y) :x(x), y(y) {}
	Vector2D(int x, int y) :x((float)x), y((float)y) {}

	//	ベクトルの長さ
	float GetLength() const;
	//	ベクトルの正規化
	void Normalize();
	//	正規化したベクトルをreturn
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
