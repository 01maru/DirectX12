#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include <cmath>

namespace MyMath {
	//	大きい値を返す
	template<typename T>
	T mMax(T a, T b);
	//	小さい値を返す
	template<typename T>
	T mMin(T a, T b);

#pragma region 円周率
	const float PI = 3.14159265358979f;
	const float PIx2 = 6.2831853071f;
#pragma endregion
	//	ガウシアンブラー用のWeight計算
	void CalcGaussianWeightsTable(float* weights, int numWeights, float sigma);

	//	視点座標、注視点座標、上方向ベクトル
	Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);

	Matrix PerspectiveFovLH(const int winwidth, const int winheight, float fovY, float nearZ, float farZ);
	Matrix OrthoLH(const int winwidth, const int winheight, float nearZ, float farZ);

	//	ラジアン変換
	float ConvertToRad(float angle);

	//	乱数
	float GetRand(float min, float max);

	bool CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad);

	//頂点ABCで作られたポリゴンから法線を計算する。
	//Vector3D& CreatePolygonNormal(const Vector3D& a, const Vector3D& b, const Vector3D& c);

	class MatView {
	public:
		Matrix mat;
		Vector3D eye;		//	視点座標
		Vector3D target;	//	注視点座標
		Vector3D up;		//	上方向ベクトル
	public:
		MatView();
		void Init(Vector3D _eye, Vector3D _target, Vector3D _up);
		void MatUpdate();
	};

	class ObjMatrix {
	public:
		Matrix matWorld;

		Matrix matScale;
		Vector3D scale;		//	スケール

		Matrix matRot;
		Vector3D rotAngle;	//	回転(単位はラジアン)

		Matrix matTrans;
		Vector3D trans;		//	位置

	public:
		void SetMatScaling();
		void SetMatRotation();
		void SetMatTransform();
		void Initialize();
		void Update();
	};
}

