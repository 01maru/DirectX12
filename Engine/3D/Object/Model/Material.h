#pragma once
#include <string>
#include "Vector3D.h"
#include "Texture.h"
#include "ConstBuff.h"

namespace CBuff {
	struct CBufferMaterialData;
}

class Material
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	std::string name_;
	Vector3D ambient_ = { 0.3f, 0.3f, 0.3f };
	Vector3D diffuse_;
	Vector3D specular_;
	float alpha_ = 1.0f;

	std::string textureFilename_;
	wchar_t wfilepath_[128];

	ConstBuff material_;

	Texture texture_;

public:
	static Material* Create();

	void Initialize();
	void Update();
	void LoadTexture();
	void SetGraphicsRootCBuffView(uint32_t rootparaIdx);

	int32_t GetTextureHandle() { return texture_.GetHandle(); }
};

