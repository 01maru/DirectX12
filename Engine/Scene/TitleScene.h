#pragma once
#include "IScene.h"
#include "ICamera.h"
#include "MyXAudio.h"
#include "Sprite.h"
#include "Texture.h"

class TitleScene :public IScene
{
private:
	static MyXAudio* xAudio;

	int timer = 0;
	bool endScene = false;

	std::unique_ptr<ICamera> camera;
#pragma region Sprite
	static Vector2D titleSize;
	static Vector2D pressSize;
	std::unique_ptr<Sprite> titleSprite;
	std::unique_ptr<Sprite> pressSprite;
#pragma endregion

#pragma region sound

#pragma endregion

#pragma region Texture
	Texture titleG;
	Texture pressG;
#pragma endregion
public:
	TitleScene();
	~TitleScene() override;
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void DrawShadow() override;
	void Draw() override;
private:
	void MatUpdate() override;
};

