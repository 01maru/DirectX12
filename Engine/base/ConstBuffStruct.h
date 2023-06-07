#pragma once
#include "Matrix.h"

namespace CBuff
{
	struct CBuffSpriteTransform {
		Matrix mat;
	};

	struct CBuffColorMaterial {
		Vector4D color;	//	RGBA
	};
}