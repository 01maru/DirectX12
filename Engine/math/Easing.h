#pragma once

class Vector2D;
class Vector3D;
class Vector4D;

namespace Easing {

#pragma region lerp

	float lerp(float start, float end, float t);
	Vector2D lerp(const Vector2D& start, const Vector2D& end, float t);
	Vector3D lerp(const Vector3D& start, const Vector3D& end, float t);
	Vector4D lerp(const Vector4D& start, const Vector4D& end, float t);

#pragma endregion

#pragma region EaseIn

	float EaseIn(float start, float end, float t, int powNum);
	Vector2D EaseIn(const Vector2D& start, const Vector2D& end, float t, int powNum);
	Vector3D EaseIn(const Vector3D& start, const Vector3D& end, float t, int powNum);
	Vector4D EaseIn(const Vector4D& start, const Vector4D& end, float t, int powNum);

#pragma endregion

#pragma region EaseOut

	float EaseOut(float start, float end, float t, int powNum);
	Vector2D EaseOut(const Vector2D& start, const Vector2D& end, float t, int powNum);
	Vector3D EaseOut(const Vector3D& start, const Vector3D& end, float t, int powNum);
	Vector4D EaseOut(const Vector4D& start, const Vector4D& end, float t, int powNum);

#pragma endregion

#pragma region EaseInOut

	float EaseInOut(float start, float end, float t, int powNum);
	Vector2D EaseInOut(const Vector2D& start, const Vector2D& end, float t, int powNum);
	Vector3D EaseInOut(const Vector3D& start, const Vector3D& end, float t, int powNum);
	Vector4D EaseInOut(const Vector4D& start, const Vector4D& end, float t, int powNum);

#pragma endregion

#pragma region EaseInBack

	float EaseInBack(float start, float end, float t, int powNum = 1, float p1 = 1.70158f);
	Vector2D EaseInBack(const Vector2D& start, const Vector2D& end, float t, int powNum=1, float p1 = 1.70158f);
	Vector3D EaseInBack(const Vector3D& start, const Vector3D& end, float t, int powNum=1, float p1 = 1.70158f);
	Vector4D EaseInBack(const Vector4D& start, const Vector4D& end, float t, int powNum=1, float p1 = 1.70158f);

#pragma endregion

#pragma region EaseOutBack

	float EaseOutBack(float start, float end, float t, int powNum = 1, float p1 = 1.70158f);
	Vector2D EaseOutBack(const Vector2D& start, const Vector2D& end, float t, int powNum=1, float p1 = 1.70158f);
	Vector3D EaseOutBack(const Vector3D& start, const Vector3D& end, float t, int powNum=1, float p1 = 1.70158f);
	Vector4D EaseOutBack(const Vector4D& start, const Vector4D& end, float t, int powNum=1, float p1 = 1.70158f);

#pragma endregion
}

