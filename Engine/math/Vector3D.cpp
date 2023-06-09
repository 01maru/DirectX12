#include "Vector3D.h"

#include "Vector4D.h"
#include "Vector2D.h"
#include "Matrix.h"
#include <cmath>

Vector3D::Vector3D() :x(0.0f), y(0.0f), z(0.0f) {}

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3D::Vector3D(int x, int y, int z) :x((float)x), y((float)y), z((float)z) {}

Vector3D::Vector3D(const Vector2D& vec, float z) : x(vec.x), y(vec.y), z(z) {}

Vector3D::Vector3D(const Vector2D& vec, int z) :x(vec.x), y(vec.y), z((float)z) {}

Vector3D::Vector3D(const Vector4D& vec) :x(vec.x), y(vec.y), z(vec.z) {}

float Vector3D::GetLength() const
{
	return sqrt(x * x + y * y + z * z);
}

void Vector3D::Normalize()
{
	float len = GetLength();

	if (len != 0)	*this /= len;
}

Vector3D& Vector3D::GetNormalize()
{
	float len = GetLength();

	if (len != 0)	return *this /= len;
	return *this;
}

float Vector3D::dot(const Vector3D& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::cross(const Vector3D& v) const
{
	return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3D Vector3D::operator+() const
{
	return *this;
}

Vector3D Vector3D::operator-() const
{
	return Vector3D(-x, -y, -z);
}

Vector3D& Vector3D::operator+=(const Vector3D& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3D& Vector3D::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3D& Vector3D::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

bool Vector3D::operator==(const Vector3D& vec)
{
	return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z);
}

bool Vector3D::operator!=(const Vector3D& vec)
{
	return (this->x != vec.x) || (this->y != vec.y) || (this->z != vec.z);
}

const Vector3D operator+(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D temp(v1);
	return temp += v2;
}

const Vector3D operator-(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D temp(v1);
	return temp -= v2;
}

const Vector3D operator*(const Vector3D& v, float s)
{
	Vector3D temp(v);
	return temp *= s;
}

const Vector3D operator*(float s, const Vector3D& v)
{
	return v * s;
}

const Vector3D Vec3Transform(const Vector3D& v, const Matrix& m)
{
	Vector3D ans;
	ans.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	ans.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	ans.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	//ans.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	
	return ans;
}

const Vector3D Vec3TransformNormal(const Vector3D& v, const Matrix& m)
{
	Vector3D ans;
	ans.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	ans.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	ans.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	//ans.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	return ans;
}
