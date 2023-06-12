#pragma once
#include "AbstractSceneFactory.h"
#include <future>

#include "IScene.h"
#include "Sprite.h"
#include "Texture.h"
#include "PostEffect.h"

#include "ILoadingObj.h"
#include "FrameCounter.h"

class SceneManager
{
private:
	//	フェードインアウトフレーム数
	static const int S_SCENE_CHANGE_TIME = 60;

	std::unique_ptr<IScene> scene_;
	std::unique_ptr<IScene> nextScene_;

	std::unique_ptr<AbstractSceneFactory> sceneFactry_;

	Vector4D screenColor = { 1.0f,1.0f,1.0f,1.0f };

	int sceneChangeTimer = 0;

	FrameCounter sceneChangeCounter;

#pragma region SplashScreen

	bool isSplashScreen = true;
	Texture rogoUI;
	std::unique_ptr<Sprite> splashSprite;

#pragma endregion

#pragma region Loading
	std::future<void> sceneInitInfo;
	bool endLoading = false;
	bool sceneInitialized = true;

	std::unique_ptr<ILoadingObj> loadObj;
#pragma endregion

#pragma region PostEffect
	std::unique_ptr<PostEffect> mainScene;
	std::unique_ptr<PostEffect> luminnce;
	std::unique_ptr<PostEffect> xbulrluminnce;
	std::unique_ptr<PostEffect> ybulrluminnce;
	std::unique_ptr<PostEffect> shadowEffect;
	std::unique_ptr<PostEffect> xbulr;
	std::unique_ptr<PostEffect> ybulr;
#pragma endregion

	void ScreenColorUpdate();
	void SplashUpdate();
	void SceneFadeInUpdate();
	void SceneFadeOutUpdate();
	void SceneAsyncUpdate();
	void SceneUpdate();
	void ImguiUpdate();

	SceneManager() {};
	~SceneManager() {};
public:
	static SceneManager* GetInstance();
	SceneManager(const SceneManager& obj) = delete;
	SceneManager& operator=(const SceneManager& obj) = delete;

	void FirstSceneInitialize();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	void ChangeScreenColor(const Vector4D& color) { mainScene->SetColor(color); }
	void SceneChange();
	void SetNextScene(const std::string& sceneName);

	Texture GetShadowMap() { return ybulr->GetTexture(); }
};

