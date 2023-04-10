#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "PostEffect.h"
#include "SceneFactory.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include <future>
#include "Texture.h"

class SceneManager
{
private:
	//	フェードインアウトフレーム数
	static const int SCENE_CHANGE_TIME;

	MyDirectX* dx = MyDirectX::GetInstance();
	IScene* scene = nullptr;
	IScene* nextScene = nullptr;
	AbstractSceneFactory* sceneFactry = new SceneFactory();
	ImGuiManager* imguiMan = nullptr;

	//PostEffect* screen = PostEffect::GetInstance();

	Vector4D screenColor = { 1.0f,1.0f,1.0f,1.0f };

	int sceneChangeTimer = 0;

#pragma region Loading
	std::future<void> sceneInitInfo;
	bool endLoading = false;
	bool sceneInitialized = true;

	Texture loadTex;
	std::unique_ptr<Sprite> loadSprite;

	std::unique_ptr<PostEffect> shadowEffect;
	std::unique_ptr<PostEffect> shadowEffect2;
#pragma endregion

	SceneManager() {};
	~SceneManager();
public:
	static SceneManager* GetInstance();
	static void DeleteInstance();
	SceneManager(const SceneManager& obj) = delete;
	SceneManager& operator=(const SceneManager& obj) = delete;

	void Initialize();
	void Update();
	void Draw();

	void ChangeScreenColor(const Vector4D& color) { shadowEffect->SetColor(color); }
	void SceneChange();
	void SetNextScene(const std::string& sceneName);

	Texture GetShadowMap() { return shadowEffect2->GetTexture(); }
};

