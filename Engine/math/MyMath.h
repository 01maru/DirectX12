#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"

namespace MyMath {
	//	�傫���l��Ԃ�
	template<typename T>
	T mMax(T a, T b);
	//	�������l��Ԃ�
	template<typename T>
	T mMin(T a, T b);

#pragma region �~����
	const float PI = 3.14159265358979f;
	const float PIx2 = 6.2831853071f;
#pragma endregion
	//	�K�E�V�A���u���[�p��Weight�v�Z
	void CalcGaussianWeightsTable(float* weights, int numWeights, float sigma);

	//	���_���W�A�����_���W�A������x�N�g��
	Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);

	Matrix PerspectiveFovLH(const int winwidth, const int winheight, float fovY, float nearZ, float farZ);
	Matrix OrthoLH(const int winwidth, const int winheight, float nearZ, float farZ);

	//	���W�A���ϊ�
	float ConvertToRad(float angle);

	//	����
	float GetRand(float min, float max);

	bool CollisionCircleLay(const Vector3D& startL, const Vector3D& endL, const Vector3D& pos, float rad);

	//���_ABC�ō��ꂽ�|���S������@�����v�Z����B
	Vector3D CreatePolygonNormal(const Vector3D& a, const Vector3D& b, const Vector3D& c);

	class MatView {
	public:
		Matrix mat;
		Vector3D eye;		//	���_���W
		Vector3D target;	//	�����_���W
		Vector3D up;		//	������x�N�g��
	public:
		MatView();
		void Init(const Vector3D& _eye, const Vector3D& _target, const Vector3D& _up);
		void MatUpdate();
	};

	class ObjMatrix {
	public:
		Matrix matWorld;

		Matrix matScale;
		Vector3D scale;		//	�X�P�[��

		Matrix matRot;
		Vector3D rotAngle;	//	��](�P�ʂ̓��W�A��)

		Matrix matTrans;
		Vector3D trans;		//	�ʒu

	public:
		void SetMatScaling();
		void SetMatRotation();
		void SetMatTransform();
		void Initialize();
		void Update();
	};

	class SpriteMatrix
	{
	private:
		Matrix matWorld;

		Matrix matRot;
		float rotAngle = 0.0f;

		Matrix matTrans;
		Vector2D trans_;
	public:
		void SetMatRotation();
		void SetMatTransform();
		void Update();

		const Matrix& GetMatWorld() { return matWorld; }
		float GetAngle() { return rotAngle; }
		const Vector2D& GetTrans() { return trans_; }

		void SetAngle(float angle) { rotAngle = angle; }
		void SetTrans(const Vector2D& trans) { trans_ = trans; }
	};
}

