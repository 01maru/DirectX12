#include "SpotLight.h"
#include <cmath>

void SpotLight::SetLightDir(const Vector3D& dir)
{
	dir_ = dir;
	dir_.Normalize();
}

void SpotLight::SetLightFactorAngle(const Vector2D& factorAngle)
{
	factorAngleCos_.x = cosf(MyMath::ConvertToRad(factorAngle.x));
	factorAngleCos_.y = cosf(MyMath::ConvertToRad(factorAngle.y));
}
