#pragma once
#include "MyMath.h"
#include "VertIdxBuff.h"
#include "ConstBuff.h"

namespace CBuff {
	struct CBuffColorMaterial;
	struct CBufferParticleTransform;
}

class Particle :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region CBuff

	ConstBuff transform_;
	CBuff::CBufferParticleTransform* cTransformMap_ = nullptr;

	ConstBuff colorMaterial_;
	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;

#pragma endregion

	Vector3D vertex_;
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };

	float scale_ = 1.0f;

	bool isBillboard_ = false;
	bool isBillboardY_ = false;

private:
	void SetVertices() override;
	void TransferVertex();

public:
	Particle();
	Particle(const Vector3D& pos);

	void Initialize();
	void MatUpdate();
	void Draw(int32_t handle);

	//	Getter
	const Vector3D& GetPosition() { return vertex_; }
	const Vector4D& GetColor() { return color_; }
	float GetScale() { return scale_; }
	bool IsBillboard() { return isBillboard_; }
	bool IsBillboardY() { return isBillboardY_; }

	//	Setter
	void SetPosition(const Vector3D& pos);
	void SetColor(const Vector4D& color) { color_ = color; }
	void SetScale(float scale);
	void SetIsBillboard(bool flag) { isBillboard_ = flag; }
	void SetIsBillboardY(bool flag) { isBillboardY_ = flag; }

	void Move(const Vector3D& spd);
};

