#pragma once
#include "IScene.h"
#include "SceneFactory.h"
#include <future>
#include "Sprite.h"
#include "Texture.h"
#include "PostEffect.h"

#include "ILoadingObj.h"

class SceneManager
{
private:
	//	フェードインアウトフレーム数
	static const int S_SCENE_CHANGE_TIME = 60;

	IScene* scene = nullptr;
	IScene* nextScene = nullptr;
	std::unique_ptr<AbstractSceneFactory> sceneFactry;

	Vector4D screenColor = { 1.0f,1.0f,1.0f,1.0f };

	int sceneChangeTimer = 0;

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

	void ImguiUpdate();

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
	void SceneChange();
	void SetNextScene(const std::string& sceneName);

	Texture GetShadowMap() { return ybulr->GetTexture(); }
};

