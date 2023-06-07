#pragma once
#include "IScene.h"

#include "IModel.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Object2D.h"
#include "VolumeLight.h"
#include "Player.h"
#include "ICamera.h"
#include "Texture.h"


class CollisionManager;

class GameScene :public IScene
{
private:
	ICamera* camera = nullptr;

	CollisionManager* collisionMan = nullptr;

	bool isDebug = true;
#pragma region Model
	std::unique_ptr<IModel> modelSkydome;
	std::unique_ptr<IModel> modelGround;
#pragma endregion

#pragma region Sprite
	std::unique_ptr<Sprite> sprite;
#pragma endregion
	int bgmSound = -1;
#pragma region Texture
	Texture reimuG;
	Texture grassG;
#pragma endregion
	std::unique_ptr<Object3D> skydome;
	std::unique_ptr<Object3D> ground;
	std::unique_ptr<Player> player;

	float lightColor = 1.0f;
public:	
	GameScene();
	~GameScene() override;
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void DrawShadow() override;
	void Draw() override;
private:
	void CollisionUpdate();
	void MatUpdate() override;
};

