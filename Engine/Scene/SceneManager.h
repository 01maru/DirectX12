#pragma once
#include "IScene.h"
#include "SceneFactory.h"
#include <future>
#include "Sprite.h"
#include "Texture.h"
#include "PostEffect.h"

class SceneManager
{
private:
	//	フェードインアウトフレーム数
	static const int SCENE_CHANGE_TIME;

	IScene* scene = nullptr;
	IScene* nextScene = nullptr;
	std::unique_ptr<AbstractSceneFactory> sceneFactry;

	Vector4D screenColor = { 1.0f,1.0f,1.0f,1.0f };

	int sceneChangeTimer = 0;

#pragma region Loading
	std::future<void> sceneInitInfo;
	bool endLoading = false;
	bool sceneInitialized = true;

	Texture loadTex;
	std::unique_ptr<Sprite> loadSprite;
#pragma endregion

#pragma region PostEffect
	std::unique_ptr<PostEffect> screen;
	std::unique_ptr<PostEffect> shadowEffect;
	std::unique_ptr<PostEffect> xbulr;
	std::unique_ptr<PostEffect> ybulr;
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

	void ChangeScreenColor(const Vector4D& color) { screen->SetColor(color); }
	void SceneChange();
	void SetNextScene(const std::string& sceneName);

	Texture GetShadowMap() { return ybulr->GetTexture(); }
};

