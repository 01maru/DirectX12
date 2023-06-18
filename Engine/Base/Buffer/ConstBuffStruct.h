#pragma once
#include "Vector2D.h"
#include "Vector4D.h"
#include "Matrix.h"

namespace CBuff
{
	const int MAX_BONES = 32;

	struct CBuffSpriteTransform {
		Matrix mat;
	};

	struct CBufferParticleTransform {
		Matrix mat;
		Matrix matBillboard;
		Vector3D cameraPos;
		float scale;
	};

	struct CBuffColorMaterial {
		Vector4D color;	//	RGBA
	};

	struct CBuffObj3DTransform {
		Matrix matViewProj;
		Matrix matWorld;
		Vector3D cameraPos;
	};

	struct CBuffLightMaterial {
		Matrix mLVP;
		Vector3D cameraPos;
	};

	struct CBuffSkinData
	{
		Matrix bones[MAX_BONES];
	};

	struct CBufferBlurWeight {
		Vector4D weight[2];
	};

	struct CBufferMaterialData {
		Vector3D ambient;
		float pad1;
		Vector3D diffuse;
		float pad2;
		Vector3D specular;
		float alpha;
	};

#pragma region Light

	struct CBuffPointLight
	{
		Vector3D pos;
		float pad1;
		Vector3D color;
		float pad2;
		Vector3D lightatten;
		bool active;
	};

	struct CBuffDirLightData
	{
		Vector3D lightv;
		float pad1;
		Vector3D lightcolor;
		bool active;
	};

	struct CBuffDisFogData
	{
		Vector3D color;
		bool active;
		float start;
		float end;
		float fogNear;
		float fogFar;
	};

	struct CBuffSpotLightData
	{
		Vector3D lightv;
		float pad1;
		Vector3D lightpos;
		float pad2;
		Vector3D lightcolor;
		float pad3;
		Vector3D lightatten;
		float pad4;
		Vector2D lightfactoranglecos;
		bool active;
		float pad5;
	};

	struct CBuffCircleShadowData
	{
		Vector3D dir;
		float pad1;
		Vector3D casterPos;
		float distanceCasterLight;
		Vector3D atten;
		float pad2;
		Vector2D factorAngleCos;
		bool active;
		float pad3;
	};

#pragma endregion
}