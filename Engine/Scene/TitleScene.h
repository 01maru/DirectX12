#pragma once
#include "IScene.h"
#include "Sprite.h"

#include "ICamera.h"

#include "Texture.h"

class TitleScene :public IScene
{
private:
	int timer = 0;
	bool endScene = false;

	std::unique_ptr<ICamera> camera;

#pragma region Sprite
	std::unique_ptr<Sprite> titleSprite;
	std::unique_ptr<Sprite> pressSprite;
	std::unique_ptr<Sprite> backSprite;
#pragma endregion

#pragma region sound
	int titleBGM = -1;
	int cursorMoveSound = -1;
	int decisionSound = -1;
#pragma endregion

#pragma region Texture
	Texture titleG;
	Texture pressG;
#pragma endregion

public:
	~TitleScene() override {};
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void ImguiUpdate() override;
	void DrawShadow() override;
	void Draw() override;
private:
	void MatUpdate() override;
};

