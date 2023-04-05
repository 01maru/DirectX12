#pragma once
#include "Object2D.h"
#include "Texture.h"

class VolumeLightObj
{
private:
	static Texture handle;

	Object2D lightObj[2];
	
public:
	VolumeLightObj(const Vector2D& scale_, const Vector3D& pos, float angle);
	static void SetLightGraph(Texture handle_);
	void Initialize(const Vector2D& scale_, const Vector3D& pos, float angle);
	void MatUpdate();
	void Draw();
};

