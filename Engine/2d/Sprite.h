#pragma once
#include "VertIdxBuff.h"
#include "ConstBuff.h"

#include "Texture.h"
#include "MyMath.h"
#include <vector>

namespace CBuff {
	struct CBuffColorMaterial;
	struct CBuffSpriteTransform;
}

class GPipeline;

class Sprite :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Matrix mat2D;
	
	//	�摜�𒣂�t����|���S���̐ݒ�
	MyMath::SpriteMatrix mat;
	Vector2D size = { 100.0f,100.0f };
	Vector2D anchorPoint;

	//	�\������摜�̐ݒ�
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
	//	�s��
	ConstBuff transform;
	CBuff::CBuffSpriteTransform* constMapTransform = nullptr;
	//	�F
	ConstBuff colorMaterial;
	CBuff::CBuffColorMaterial* mapMaterial = nullptr;
#pragma endregion

private:
	void SetVertices() override;

	void MatUpdate();
	//	vertices�̍��W�ݒ�
	void SetVerticesPos();
	//	vertices��UV�ݒ�
	void SetVerticesUV();
	//	vertices�]��
	void TransferVertex();

	//	�摜�T�C�Y���擾����
	void AdjustTextureSize();
public:
	static void StaticInitialize();

	void Initialize(Texture texture_);
	void Update();
	void Draw(GPipeline* pipeline = nullptr);

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

