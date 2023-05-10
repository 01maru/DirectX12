#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "Input.h"
#include "Shader.h"
#include "IModel.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Object2D.h"
//#include "Particle.h"
#include "TouchableObject.h"
#include "VolumeLight.h"
#include "Player.h"
#include "ICamera.h"
#include "Texture.h"
#include "Grass.h"


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
	std::unique_ptr<IModel> modelSphere;
	std::unique_ptr<IModel> modelSword;
#pragma endregion

#pragma region Sprite
	
#pragma endregion

#pragma region Texture

#pragma endregion

	std::unique_ptr<Object3D> skydome;
	std::unique_ptr<Object3D> ground;
	std::unique_ptr<Object3D> sphere;
	std::unique_ptr<Object3D> player;
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
	MyDirectX* dx = MyDirectX::GetInstance();
	Input* input = Input::GetInstance();

	void CollisionUpdate();
	void MatUpdate() override;
};

