#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "Input.h"
#include "Shader.h"
#include "IModel.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Object2D.h"
#include "Particle.h"
#include "TouchableObject.h"
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
	std::unique_ptr<IModel> modelHill;
	std::unique_ptr<IModel> modelSword;
	std::unique_ptr<IModel> modelTree;
	std::unique_ptr<IModel> modelTree2;
#pragma endregion

#pragma region Sprite
	std::unique_ptr<Sprite> sprite;

	std::vector<Particle> grass;
#pragma endregion
	int bgmSound = -1;
#pragma region Texture
	Texture reimuG;
	Texture grassG;
#pragma endregion
	std::unique_ptr<Object3D> skydome;
	std::unique_ptr<TouchableObject> ground;
	std::unique_ptr<Object3D> hill;
	std::unique_ptr<Object3D> tree;
	std::vector<Object3D*> tree2;
	std::unique_ptr<Player> player;
	std::vector<VolumeLightObj> testVolLight;

	int mord = 0;

public:
	enum TaskMord
	{
		Phong,
		PointLight,
		SpotLight,
		CircleShadow,
	};
	
	GameScene();
	~GameScene() override;
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void Draw() override;
private:
	MyDirectX* dx = MyDirectX::GetInstance();
	Input* input = Input::GetInstance();

	void CollisionUpdate();
	void MatUpdate() override;
};

