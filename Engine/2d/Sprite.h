#pragma once
#include "VertIdxBuff.h"
#include "ConstBuff.h"
#include "ConstBuffStruct.h"

#include "Texture.h"
#include "MyMath.h"
#include <vector>

class Sprite :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Matrix mat2D;
	
	//	画像を張り付けるポリゴンの設定
	MyMath::SpriteMatrix mat;
	Vector2D size = { 100.0f,100.0f };
	Vector2D anchorPoint;

	//	表示する画像の設定
	Vector2D textureLeftTop;
	Vector2D textureSize = { 100.0f,100.0f };
	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };
	bool isFlipX = false;
	bool isFlipY = false;

	Texture handle;

	bool isInvisible = false;
	bool dirtyFlagUV = true;
	bool dirtyFlagPos = true;
	bool dirtyFlagMat = true;
	bool dirtyFlagColor = true;

	std::vector<ScreenVertex> vertices;

	enum VertexNumber {
		LB,
		LT,
		RB,
		RT,
	};

#pragma region CBuff
	//	行列
	ConstBuff transform;
	CBuff::CBuffSpriteTransform* constMapTransform = nullptr;
	//	色
	ConstBuff colorMaterial;
	CBuff::CBuffColorMaterial* mapMaterial = nullptr;
#pragma endregion

private:
	void SetVertices() override;

	void MatUpdate();
	//	verticesの座標設定
	void SetVerticesPos();
	//	verticesのUV設定
	void SetVerticesUV();
	//	vertices転送
	void TransferVertex();

	//	画像サイズを取得する
	void AdjustTextureSize();
public:
	static void StaticInitialize();

	void Initialize(Texture texture_);
	void Update();
	void Draw();

	//	Getter
	const Vector2D& GetPosition() { return mat.GetTrans(); }
	const Vector2D& GetSize() const { return size; }
	float GetRotation() { return mat.GetAngle(); }
	const Vector4D& GetColor() const { return color; }
	const Vector2D& GetTextureSize() const { return textureSize; }
	const Vector2D& GetTextureLeftTop() const { return textureLeftTop; }

	//	Setter
	void SetPosition(const Vector2D& position);
	void SetRotation(float rotation);
	
	void SetColor(const Vector4D& color_);
	
	void SetSize(const Vector2D& size_);
	void SetAnchorPoint(const Vector2D& anchor);

	void SetTextureLeftTop(const Vector2D& leftTop);
	void SetTextureSize(const Vector2D& size_);
	void SetHandle(Texture handle_) { handle = handle_; }
};

