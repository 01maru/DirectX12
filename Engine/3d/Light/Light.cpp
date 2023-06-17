#include "Light.h"
#include <cassert>

Light* Light::GetInstance()
{
	static Light instance;
	return &instance;
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	ConstBufferLightData* constMap = nullptr;
	result = constBuff_.GetResource()->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor_;

		for (size_t i = 0; i < sDIRLIGHT_NUM; i++)
		{
			if (dirLights_[i].GetIsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights_[i].GetLightColor();
			}
			else {
				constMap->dirLights[i].active = 0;
			}
		}

		for (size_t i = 0; i < sPOINTLIGHT_NUM; i++)
		{
			if (pointLights_[i].GetIsActive()) {
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].pos = pointLights_[i].GetLightPos();
				constMap->pointLights[i].color = pointLights_[i].GetLightColor();
				constMap->pointLights[i].lightatten = pointLights_[i].GetLightAtten();
			}
			else {
				constMap->pointLights[i].active = 0;
			}
		}

		for (size_t i = 0; i < sSPOTLIGHT_NUM; i++)
		{
			if (spotLights_[i].GetIsActive()) {
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightpos= spotLights_[i].GetLightPos();
				constMap->spotLights[i].lightcolor = spotLights_[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights_[i].GetLightAtten();
				constMap->spotLights[i].lightv = spotLights_[i].GetLightDir();
				constMap->spotLights[i].lightfactoranglecos = spotLights_[i].GetLightFactorAngleCos();
			}
			else {
				constMap->spotLights[i].active = 0;
			}
		}

		for (size_t i = 0; i < sCIRCLESHADOW_NUM; i++)
		{
			if (circleShadows_[i].GetIsActive()) {
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = circleShadows_[i].GetDir();
				constMap->circleShadows[i].casterPos= circleShadows_[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight= circleShadows_[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows_[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos= circleShadows_[i].GetFactorAngleCos();
			}
			else {
				constMap->circleShadows[i].active = 0;
			}
		}

		if (distanceFog_.GetIsActive()) {
			constMap->distanceFog.active = 1;
			constMap->distanceFog.color = distanceFog_.GetColor();
			constMap->distanceFog.start = distanceFog_.GetStart();
			constMap->distanceFog.end = distanceFog_.GetEnd();
			constMap->distanceFog.fogFar = distanceFog_.GetFar();
			constMap->distanceFog.fogNear = distanceFog_.GetNear();
		}
		else {
			constMap->distanceFog.active = 0;
		}

		constBuff_.GetResource()->Unmap(0, nullptr);
	}
}

void Light::Initialize()
{
	constBuff_.Initialize((sizeof(ConstBufferLightData) + 0xFF) & ~0xFF);

	TransferConstBuffer();
}

void Light::Update()
{
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void Light::Draw()
{
	constBuff_.SetGraphicsRootCBuffView(3);
}

void Light::SetGraphicsRootCBuffView(int32_t lootparaIdx)
{
	constBuff_.SetGraphicsRootCBuffView(lootparaIdx);
}

void Light::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	dirLights_[index].SetActive(active);
}

void Light::SetDirLightDir(int32_t index, const Vector3D& lightdir_)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	dirLights_[index].SetLightDir(lightdir_);
	dirty_ = true;
}

void Light::SetDirLightColor(int32_t index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < sDIRLIGHT_NUM);
	dirLights_[index].SetLightColor(lightcolor_);
	dirty_ = true;
}

void Light::SetDirLightShadow(int32_t index, bool shadowflag)
{
	dirLights_[index].SetShadow(shadowflag);
}

void  Light::SetPointLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);
	pointLights_[index].SetActive(active);
}

void  Light::SetPointLightPos(int32_t index, const Vector3D& lightpos)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);
	pointLights_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void  Light::SetPointLightColor(int32_t index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);
	pointLights_[index].SetLightColor(lightcolor_);
	dirty_ = true;
}

void  Light::SetPointLightAtten(int32_t index, const Vector3D& lightAtten)
{
	assert(0 <= index && index < sPOINTLIGHT_NUM);
	pointLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void Light::SetSpotLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);
	spotLights_[index].SetActive(active);
}

void Light::SetSpotLightDir(int32_t index, const Vector3D& lightdir_)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);
	spotLights_[index].SetLightDir(lightdir_);
	dirty_ = true;
}

void Light::SetSpotLightPos(int32_t index, const Vector3D& lightpos)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);
	spotLights_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void Light::SetSpotLightColor(int32_t index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);
	spotLights_[index].SetLightColor(lightcolor_);
	dirty_ = true;
}

void Light::SetSpotLightAtten(int32_t index, const Vector3D& lightAtten)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);
	spotLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void Light::SetSpotLightFactorAngle(int32_t index, const Vector2D& lightFactorAngle)
{
	assert(0 <= index && index < sSPOTLIGHT_NUM);
	spotLights_[index].SetLightFactorAngle(lightFactorAngle);
	dirty_ = true;
}

void Light::SetCircleShadowActive(int32_t index, bool active)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);
	circleShadows_[index].SetActive(active);
}

void Light::SetCircleShadowCasterPos(int32_t index, const Vector3D& casterPos_)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);
	circleShadows_[index].SetCasterPos(casterPos_);
	dirty_ = true;
}

void Light::SetCircleShadowDir(int32_t index, const Vector3D& dir_)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);
	circleShadows_[index].SetDir(dir_);
	dirty_ = true;
}

void Light::SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);
	circleShadows_[index].SetDistanceCasterLight(distanceCasterLight);
	dirty_ = true;
}

void Light::SetCircleShadowAtten(int32_t index, const Vector3D& atten)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);
	circleShadows_[index].SetAtten(atten);
	dirty_ = true;
}
void Light::SetCircleShadowFactorAngle(int32_t index, const Vector2D& factorAngle)
{
	assert(0 <= index && index < sCIRCLESHADOW_NUM);
	circleShadows_[index].SetFactorAngle(factorAngle);
	dirty_ = true;
}