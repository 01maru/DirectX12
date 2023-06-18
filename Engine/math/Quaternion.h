#pragma once

class Vector3D;
class Matrix;

class Quaternion
{
public:
	float x = 0.0f;	//	虚数
	float y = 0.0f;	//	虚数
	float z = 0.0f;	//	虚数
	float w = 0.0f;	//	実数

public:
	Quaternion() {};
	Quaternion(float w, float x, float y, float z) :w(w), x(x), y(y), z(z) {};
	Quaternion(float w, const Vector3D& vec);
	Quaternion SetQuaternion(const Vector3D& v);

	Vector3D GetVector3();
	void SetVec(const Vector3D& vec);

	Quaternion CalcProduct(const Quaternion& q, const Quaternion& r);
	Quaternion GetIdentity();
	Quaternion GetConjugation();
	void Normalize();

	Matrix GetRotMatrix();


	Quaternion& operator/=(float s);
	Quaternion& operator*=(float s);
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);

	float Dot(const Quaternion& q);
};

Quaternion SetQuaternion(const Vector3D& v, float angle);

Quaternion operator-(const Quaternion& q);

Quaternion operator*(const Quaternion& q, float s);
Quaternion operator*(float s, const Quaternion& q);
Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
Quaternion operator-(const Quaternion& q1, const Quaternion& q2);

Quaternion Multiply(const Quaternion& q, const Quaternion& r);
Quaternion IdentityQuaternion();
Quaternion Conjugate(const Quaternion& quaternion);
float Norm(const Quaternion& quaternion);
Quaternion Normalize(const Quaternion& quaternion);
Quaternion Inverse(const Quaternion& quaternion);
Quaternion MakeAxisAngle(const Vector3D& axis, float angle);
Vector3D RotateVector(const Vector3D& vec, const Quaternion& quaternion);
Matrix MakeRotateMatrix(const Quaternion& quaternion);

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
Quaternion CameraSlerp(const Quaternion& q0, const Quaternion& q1, float t);

Quaternion DirectionToDirection(const Vector3D& u, const Vector3D& v);