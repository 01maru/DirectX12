#pragma once
#include "AbstractSceneFactory.h"
#include "IScene.h"
#include "PostEffect.h"
#include <future>

#include "SplashSprite.h"
#include "ILoadingObj.h"
#include "FrameCounter.h"

class SceneManager
{
private:
	FrameCounter sceneChangeCounter_;

	std::unique_ptr<IScene> scene_;
	std::unique_ptr<IScene> nextScene_;

	std::unique_ptr<AbstractSceneFactory> sceneFactry_;

	Vector4D screenColor_ = { 1.0f,1.0f,1.0f,1.0f };

#pragma region SplashScreen

	bool isSplashScreen_ = false;
	std::unique_ptr<SplashSprite> splashSprite_;

#pragma endregion

#pragma region Loading
	std::future<void> sceneInitInfo_;
	bool endLoading_ = false;
	bool sceneInitialized_ = true;

	std::unique_ptr<ILoadingObj> loadObj_;
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

private:	//	ŠÖ”
	void ScreenColorUpdate();
	void SplashUpdate();
	void SceneFadeInUpdate();
	void SceneFadeOutUpdate();
	void SceneAsyncUpdate();
	void SceneUpdate();
	void ImguiUpdate();

	void FirstSceneInitialize();
	void SplashScreenInitialize();
	void SceneChange();

	SceneManager() {};
	~SceneManager() {};
public:
	static SceneManager* GetInstance();
	SceneManager(const SceneManager& obj) = delete;
	SceneManager& operator=(const SceneManager& obj) = delete;

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	void ChangeScreenColor(const Vector4D& color) { mainScene->SetColor(color); }
	void SetNextScene(const std::string& sceneName);

	Texture GetShadowMap() { return ybulr->GetTexture(); }
};

