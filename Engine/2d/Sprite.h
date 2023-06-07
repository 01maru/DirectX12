#pragma once
#include "VertIdxBuff.h"
#include "Texture.h"
#include "MyMath.h"
#include <vector>

class Sprite :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Matrix mat2D;
	
	MyMath::SpriteMatrix mat;

	Vector4D color;
	Vector2D size = { 100.0f,100.0f };
	Vector2D anchorPoint;

	Vector2D textureLeftTop;
	Vector2D textureSize = { 100.0f,100.0f };

	Texture handle;

	bool isFlipX = false;
	bool isFlipY = false;
	bool isInvisible = false;

	std::vector<ScreenVertex> vertices;

	enum VertexNumber {
		LB,
		LT,
		RB,
		RT,
	};
	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};

	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;
public:
	static void StaticInitialize();

	Sprite() {};
	Sprite(Texture texture_);
	void Initialize(Texture texture_);
	void SetTextureRect();
	void Update();
	void MatUpdate();
	void Draw();
	void DrawRect(const Vector2D& textureLeftTop, const Vector2D& textureSize);

	void TransferVertex();

	//	Getter
	const Vector2D& GetPosition() { return mat.GetTrans(); }
	const Vector2D& GetSize() const { return size; }
	float GetRotation() { return mat.GetAngle(); }
	const Vector4D& GetColor() const { return color; }
	const Vector2D& GetTextureSize() const { return textureSize; }
	const Vector2D& GetTextureLeftTop() const { return textureLeftTop; }

	//	Setter
	void SetPosition(const Vector2D& position) { mat.SetTrans(position); }
	void SetSize(const Vector2D& size_) { size = size_; }
	void SetAnchorPoint(const Vector2D& anchor) { anchorPoint = anchor; }
	void SetTextureLeftTop(const Vector2D& leftTop) { textureLeftTop = leftTop; }
	void SetTextureSize(const Vector2D& size_) { textureSize = size_; }
	void SetRotation(float rotation) { mat.SetAngle(rotation); }
	void SetColor(const Vector4D& color_) { color = color_; }
	void SetHandle(Texture handle_) { handle = handle_; }
private:
	void SetVertices() override;

	void AdjustTextureSize();
};

