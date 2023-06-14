#include "Vector2D.h"
#include <cmath>

Vector2D Vector2D::operator+() const
{
	return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

float Vector2D::GetLength() const
{
	return sqrt(x * x + y * y);
}

void Vector2D::Normalize()
{
	float len = GetLength();

	//	0ベクトルじゃなかったら
	if (len != 0)	*this /= len;
}

Vector2D& Vector2D::GetNormalize()
{
	float len = GetLength();

	//	0ベクトルじゃなかったら
	if (len != 0)	return *this /= len;

	return *this;
}

Vector2D& Vector2D::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

Vector2D& Vector2D::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

bool Vector2D::operator!=(const Vector2D& vec)
{
	return (this->x != vec.x) || (this->y != vec.y);
}

bool Vector2D::operator==(const Vector2D& vec)
{
	return (this->x == vec.x) && (this->y == vec.y);
}

const Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
	Vector2D temp(v1);
	return temp -= v2;
}

const Vector2D operator*(const Vector2D& v, float s)
{
	Vector2D temp(v);
	return temp *= s;
}

const Vector2D operator*(float s, const Vector2D& v)
{
	return v * s;
}
