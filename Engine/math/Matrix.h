#pragma once

class Matrix
{
public:
	float m[4][4];
public:
	//	コンストラクタ
	Matrix();
	//	単位行列
	void Identity();

	Matrix& operator*=(const Matrix& m2);
};

const Matrix operator*(const Matrix& m1, const Matrix& m2);
//	逆行列
bool InverseMatrix(const Matrix& mat, Matrix& invMat);

//	2D座標系変換用行列
Matrix Create2DTransformMatrix();
