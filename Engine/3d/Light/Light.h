#pragma once
#include "PointLight.h"
#include "DirLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "DistanceFog.h"

#include "ConstBuff.h"
#include "ConstBuffStruct.h"

class Light
{
private:
	static const size_t sDIRLIGHT_NUM = 3;
	static const size_t sPOINTLIGHT_NUM = 3;
	static const size_t sSPOTLIGHT_NUM = 3;
	static const size_t sCIRCLESHADOW_NUM = 1;

	struct ConstBufferLightData
	{
		Vector3D ambientColor;
		float pad1;
		CBuff::CBuffDirLightData dirLights[sDIRLIGHT_NUM];
		CBuff::CBuffPointLight pointLights[sPOINTLIGHT_NUM];
		CBuff::CBuffSpotLightData spotLights[sSPOTLIGHT_NUM];
		CBuff::CBuffCircleShadowData circleShadows[sCIRCLESHADOW_NUM];
		CBuff::CBuffDisFogData distanceFog;
	};

	ConstBuff constBuff_;


	bool dirty_ = false;
	Vector3D ambientColor_ = { 1,1,1 };
	PointLight pointLights_[sPOINTLIGHT_NUM];
	DirLight dirLights_[sDIRLIGHT_NUM];
	SpotLight spotLights_[sSPOTLIGHT_NUM];
	CircleShadow circleShadows_[sCIRCLESHADOW_NUM];
	DistanceFog distanceFog_;

private:
	Light() {};
	~Light() {};

public:
	static Light* GetInstance();
	Light(const Light& obj) = delete;
	Light& operator=(const Light& obj) = delete;

	void Initialize();
	void TransferConstBuffer();
	void Update();
	void Draw();
	void SetGraphicsRootCBuffView(int32_t lootparaIdx);

	void SetDirLightActive(int32_t index, bool active);
	void SetDirLightDir(int32_t index, const Vector3D& lightdir_);
	void SetDirLightColor(int32_t index, const Vector3D& lightcolor_);
	void SetDirLightShadow(int32_t index, bool shadowflag);
	ICamera* GetDirLightCamera(int32_t index) { return dirLights_[index].GetCamera(); }

	void SetPointLightActive(int32_t index, bool active);
	void SetPointLightPos(int32_t index, const Vector3D& lightpos);
	void SetPointLightColor(int32_t index, const Vector3D& lightcolor_);
	void SetPointLightAtten(int32_t index, const Vector3D& lightAtten);

	void SetSpotLightActive(int32_t index, bool active);
	void SetSpotLightDir(int32_t index, const Vector3D& lightdir_);
	void SetSpotLightPos(int32_t index, const Vector3D& lightpos);
	void SetSpotLightColor(int32_t index, const Vector3D& lightcolor_);
	void SetSpotLightAtten(int32_t index, const Vector3D& lightAtten);
	void SetSpotLightFactorAngle(int32_t index, const Vector2D& lightFactorAngle);

	void SetCircleShadowActive(int32_t index, bool active);
	void SetCircleShadowCasterPos(int32_t index, const Vector3D& casterPos_);
	void SetCircleShadowDir(int32_t index, const Vector3D& dir_);
	void SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight);
	void SetCircleShadowAtten(int32_t index, const Vector3D& atten);
	void SetCircleShadowFactorAngle(int32_t index, const Vector2D& factorAngle);

	void SetFogActive(bool active) { distanceFog_.SetActive(active); }
	void SetFogStart(float start) { distanceFog_.SetStart(start); }
	void SetFogEnd(float end) { distanceFog_.SetEnd(end); }
	void SetFogNear(float fogNear) { distanceFog_.SetNear(fogNear); }
	void SetFogFar(float fogFar) { distanceFog_.SetFar(fogFar); }
};

