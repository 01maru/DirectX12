#pragma once

class Matrix
{
public:
	float m[4][4];
public:
	//	�R���X�g���N�^
	Matrix();
	//	�P�ʍs��
	void Identity();

	Matrix& operator*=(const Matrix& m2);
};

const Matrix operator*(const Matrix& m1, const Matrix& m2);
//	�t�s��
bool InverseMatrix(const Matrix& mat, Matrix& invMat);

//	2D���W�n�ϊ��p�s��
Matrix Create2DTransformMatrix();
