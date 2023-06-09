#pragma once
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "Shader.h"

class SpriteCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Matrix mat2D;

	SpriteCommon();
	~SpriteCommon() {};
public:
	static SpriteCommon* GetInstance();

	SpriteCommon(const SpriteCommon& obj) = delete;
	SpriteCommon& operator=(const SpriteCommon& obj) = delete;

	void Initialize();
	void Draw();

	const Matrix& Get2DMat() const { return mat2D; }
};

