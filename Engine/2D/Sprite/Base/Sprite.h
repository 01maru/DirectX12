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

	static Matrix sMat2DTransform;
	
	//	�摜�𒣂�t����|���S���̐ݒ�
	MyMath::SpriteMatrix mat_;
	Vector2D size_ = { 100.0f,100.0f };
	Vector2D anchorPoint_;

	//	�\������摜�̐ݒ�
	Vector2D textureLeftTop_;
	Vector2D textureSize_ = { 100.0f,100.0f };
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
	bool isFlipX_ = false;
	bool isFlipY_ = false;

	Texture handle_;

	bool isInvisible_ = false;

	//	dirtyFlag
	bool dirtyFlagUV_ = true;
	bool dirtyFlagPos_ = true;
	bool dirtyFlagMat_ = true;
	bool dirtyFlagColor_ = true;

#pragma region VertBuff
	//	���_���
	std::vector<ScreenVertex> vertices_;

	enum VertexNumber {
		LB,
		LT,
		RB,
		RT,
	};
#pragma endregion

#pragma region CBuff
	//	�s��
	ConstBuff cbTransform_;
	CBuff::CBuffSpriteTransform* cbTransformMat_ = nullptr;
	//	�F
	ConstBuff cbColorMaterial_;
	CBuff::CBuffColorMaterial* cbMaterialMap_ = nullptr;
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
	
	void Initialize(Texture texture);
	void Update();
	void Draw(GPipeline* pipeline = nullptr);

	//	Getter
	const Vector2D& GetPosition() { return mat_.GetTrans(); }
	const Vector2D& GetSize() const { return size_; }
	float GetRotation() { return mat_.GetAngle(); }
	const Vector4D& GetColor() const { return color_; }
	const Vector2D& GetTextureSize() const { return textureSize_; }
	const Vector2D& GetTextureLeftTop() const { return textureLeftTop_; }

	//	Setter
	void SetPosition(const Vector2D& position);
	void SetRotation(float rotation);
	
	void SetColor(const Vector4D& color);
	
	void SetSize(const Vector2D& size);
	void SetAnchorPoint(const Vector2D& anchor);

	void SetTextureLeftTop(const Vector2D& leftTop);
	void SetTextureSize(const Vector2D& size);
	void SetHandle(Texture handle) { handle_ = handle; }
};

