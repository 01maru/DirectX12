#pragma once
#include "Matrix.h"

namespace CBuff
{
	const int MAX_BONES = 32;

	struct CBuffSpriteTransform {
		Matrix mat;
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

	//	Light
	struct CBuffPointLight
	{
		Vector3D pos;
		float pad1;
		Vector3D color;
		float pad2;
		Vector3D lightatten;
		size_t active;
	};
}