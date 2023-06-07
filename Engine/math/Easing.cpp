#include "Easing.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include <math.h>

#pragma region lerp

float Easing::lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

Vector2D Easing::lerp(const Vector2D& start, const Vector2D& end, float t)
{
	Vector2D ans;

	ans.x = lerp(start.x, end.x, t);
	ans.y = lerp(start.y, end.y, t);

	return ans;
}

Vector3D Easing::lerp(const Vector3D& start, const Vector3D& end, float t)
{
	Vector3D ans;

	ans.x = lerp(start.x, end.x, t);
	ans.y = lerp(start.y, end.y, t);
	ans.z = lerp(start.z, end.z, t);

	return ans;
}

Vector4D Easing::lerp(const Vector4D& start, const Vector4D& end, float t)
{
	Vector4D ans;

	ans.x = lerp(start.x, end.x, t);
	ans.y = lerp(start.y, end.y, t);
	ans.z = lerp(start.z, end.z, t);
	ans.w = lerp(start.w, end.w, t);

	return ans;
}

#pragma endregion

#pragma region EaseIn

float Easing::EaseIn(float start, float end, float t, int powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}

Vector2D Easing::EaseIn(const Vector2D& start, const Vector2D& end, float t, int powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}

Vector3D Easing::EaseIn(const Vector3D& start, const Vector3D& end, float t, int powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}

Vector4D Easing::EaseIn(const Vector4D& start, const Vector4D& end, float t, int powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}
#pragma endregion

#pragma region EaseOut

float Easing::EaseOut(float start, float end, float t, int powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

Vector2D Easing::EaseOut(const Vector2D& start, const Vector2D& end, float t, int powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

Vector3D Easing::EaseOut(const Vector3D& start, const Vector3D& end, float t, int powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

Vector4D Easing::EaseOut(const Vector4D& start, const Vector4D& end, float t, int powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

#pragma endregion

#pragma region EaseInOut

float Easing::EaseInOut(float start, float end, float t, int powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2.0f);
}

Vector2D Easing::EaseInOut(const Vector2D& start, const Vector2D& end, float t, int powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2);
}

Vector3D Easing::EaseInOut(const Vector3D& start, const Vector3D& end, float t, int powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2);
}

Vector4D Easing::EaseInOut(const Vector4D& start, const Vector4D& end, float t, int powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2);
}

#pragma endregion

#pragma region EaseInBack

float Easing::EaseInBack(float start, float end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

Vector2D Easing::EaseInBack(const Vector2D& start, const Vector2D& end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

Vector3D Easing::EaseInBack(const Vector3D& start, const Vector3D& end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

Vector4D Easing::EaseInBack(const Vector4D& start, const Vector4D& end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

#pragma endregion

#pragma region EaseOutBack

float Easing::EaseOutBack(float start, float end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

Vector2D Easing::EaseOutBack(const Vector2D& start, const Vector2D& end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

Vector3D Easing::EaseOutBack(const Vector3D& start, const Vector3D& end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

Vector4D Easing::EaseOutBack(const Vector4D& start, const Vector4D& end, float t, int powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

#pragma endregion
