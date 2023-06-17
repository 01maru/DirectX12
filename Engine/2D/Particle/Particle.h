#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "ICamera.h"
#include "DirectX.h"

#include "ConstBuff.h"

namespace CBuff {
	struct CBuffColorMaterial;
	struct CBuffSpriteTransform;
}

class Particle :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
	static ICamera* camera;
	static GPipeline* pipeline;

	struct ConstBufferDataTransform {
		Matrix mat;
		Matrix matBillboard;
		Vector3D cameraPos;
		float scale;
	};
	
#pragma region CBuff

	ConstBuff transform;
	ConstBufferDataTransform* cTransformMap = nullptr;

	ConstBuff colorMaterial;
	CBuff::CBuffColorMaterial* cColorMap = nullptr;

#pragma endregion

	Vector3D vertex;
	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };

	float scale = 1.0f;

	bool isBillboard = false;
	bool isBillboardY = false;
public:
	static void SetCamera(ICamera* camera_);
	static void SetPipeline(GPipeline* pipe);

	void Initialize();
	Particle();
	Particle(const Vector3D& pos_);
	void MatUpdate();
	void Draw(int handle);

	void SetColor(const Vector4D& color_) { color = color_; }
	const Vector4D& SetColor() { return color; }
	void SetScale(float scale_);
	void SetPosition(const Vector3D& pos);
	const Vector3D& GetPosition() { return vertex; }
	void Move(const Vector3D& spd);

	bool IsBillboard() { return isBillboard; }
	void SetIsBillboard(bool flag) { isBillboard = flag; }
	bool IsBillboardY() { return isBillboardY; }
	void SetIsBillboardY(bool flag) { isBillboardY = flag; }
private:
	void SetVertices() override;
};

