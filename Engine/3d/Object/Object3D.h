#pragma once
#define NOMINMAX
#include "GPipeline.h"
#include "Light.h"
#include "MyMath.h"
#include "IModel.h"
#include "ICamera.h"
//#include "CollisionInfo.h"
#include "DirectX.h"

#include "ConstBuff.h"

namespace CBuff {
	struct CBuffLightMaterial;
	struct CBuffObj3DTransform;
	struct CBuffSkinData;
	struct CBuffColorMaterial;
}

class BaseCollider;

class Object3D
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static GPipeline* pipeline;
	static Light* light;
	static MyDirectX* dx;
	static ICamera* camera;

#pragma region CBuff
	
	ConstBuff transform;
	CBuff::CBuffObj3DTransform* cTransformMap = nullptr;

	ConstBuff shadowTransform;
	CBuff::CBuffObj3DTransform* cShadowTransMap = nullptr;

	ConstBuff lightMaterial;
	CBuff::CBuffLightMaterial* cLightMap = nullptr;

	ConstBuff skinData;
	CBuff::CBuffSkinData* cSkinMap = nullptr;

	ConstBuff colorMaterial;
	CBuff::CBuffColorMaterial* cColorMap = nullptr;

#pragma endregion

	Object3D* parent = nullptr;
	IModel* model = nullptr;
	float animationTimer = 0.0f;
protected:
	MyMath::ObjMatrix mat;
	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };
	//BaseCollider* collider = nullptr;
	
public:
	Object3D() = default;
	virtual ~Object3D();

	static Object3D* Create(IModel* model = nullptr);

	virtual void Initialize();
	virtual void ColliderUpdate();
	void MatUpdate(ICamera* camera_ = nullptr);
	void PlayAnimation();
	virtual void DrawShadow();
	void DrawShadowReciever();
	virtual void Draw();
	void DrawSilhouette();

	static void SetLight(Light* light_);
	static void SetPipeline(GPipeline* pipeline_);
	static void SetCamera(ICamera* camera_);
	void SetModel(IModel* model_);
	//void SetCollider(BaseCollider* collider_);
	//BaseCollider* GetCollider() { return collider; }
	void SetAttribute(unsigned short attribute);

	void SetColor(const Vector4D& color_) { color = color_; }
	void SetColor(const Vector3D& color_) { color = Vector4D(color_, color.w); }
	const Vector4D& GetColor() { return color; }

	void SetAnimatonTimer(float timer) { animationTimer = timer; }
	const float GetAnimationTimer() { return animationTimer; }

	void SetPosition(const Vector3D& pos_) { mat.trans_ = pos_; }
	const Vector3D& GetPosition() { return mat.trans_; }

	void SetScale(const Vector3D& scale_) { mat.scale_ = scale_; }
	const Vector3D& GetScale() { return mat.scale_; }

	void SetRotation(const Vector3D& rot_) { mat.angle_ = rot_; }
	const Vector3D& GetRotation() { return mat.angle_; }

	inline IModel* GetModel() { return model; }
	const Matrix& GetMatWorld() { return mat.matWorld_; }

	//virtual void OnCollision(const CollisionInfo& info) { (void)info; }
};

