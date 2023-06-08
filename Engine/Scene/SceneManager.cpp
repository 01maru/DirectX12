#include "SceneManager.h"
#include "Easing.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Window.h"
#include "PipelineManager.h"

#include "LoadingSprite.h"
#include "LoadingModel.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::Initialize()
{
	mainScene = std::make_unique<PostEffect>();
	mainScene->Initialize(Window::window_width, Window::window_height, 1.0f, DXGI_FORMAT_R11G11B10_FLOAT);
	luminnce = std::make_unique<PostEffect>();
	luminnce->Initialize(Window::window_width, Window::window_height, 5.0f, DXGI_FORMAT_R11G11B10_FLOAT);
	xbulrluminnce = std::make_unique<PostEffect>();
	xbulrluminnce->Initialize(Window::window_width / 2, Window::window_height, 5.0f, DXGI_FORMAT_R11G11B10_FLOAT);
	ybulrluminnce = std::make_unique<PostEffect>();
	ybulrluminnce->Initialize(Window::window_width / 2, Window::window_height / 2, 5.0f, DXGI_FORMAT_R11G11B10_FLOAT);

	shadowEffect = std::make_unique<PostEffect>();
	shadowEffect->Initialize(Window::window_width, Window::window_height, 1.0f, DXGI_FORMAT_R32G32_FLOAT);
	
	xbulr = std::make_unique<PostEffect>();
	xbulr->Initialize(Window::window_width / 2, Window::window_height, 1.0f, DXGI_FORMAT_R32G32_FLOAT);
	ybulr = std::make_unique<PostEffect>();
	ybulr->Initialize(Window::window_width / 2, Window::window_height / 2, 1.0f, DXGI_FORMAT_R32G32_FLOAT);

	sceneFactry = std::make_unique<SceneFactory>();
	scene = sceneFactry->CreateScene("GAMESCENE");
	scene->Initialize();
	endLoading = true;

	loadTex = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/loading.png");
	loadSprite = std::make_unique<Sprite>();
	loadSprite->Initialize(loadTex);
	loadSprite->SetPosition(Vector2D{ Window::window_width - 96,Window::window_height - 98 });
	loadSprite->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	loadSprite->SetSize(Vector2D{ 64,64 });
	loadObj = std::make_unique<LoadingModel>();
	loadObj->Initialize();
	loadObj->SetIsLoading(!endLoading);

	ImGuiManager::GetInstance()->Initialize();

	//	画像転送
	MyDirectX::GetInstance()->UploadTexture();
}

void SceneManager::Finalize()
{
	scene->Finalize();
	ImGuiManager::GetInstance()->Finalize();
}

void SceneManager::Update()
{
	if (endLoading) {
		if (sceneInitialized) {

			//	フェードイン
			if (sceneChangeTimer > 0) {
				sceneChangeTimer--;

				float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)S_SCENE_CHANGE_TIME);
				screenColor.x = color;
				screenColor.y = color;
				screenColor.z = color;

				//	色設定
				mainScene->SetColor(screenColor);

				scene->MatUpdate();
			}
			else {
				//	同期処理
				scene->Update();
			}
		}
		else {
			//	nextSceneがセットされたら
			loadObj->SetIsLoading(true);

			//	フェードアウト
			if (sceneChangeTimer < S_SCENE_CHANGE_TIME) {
				sceneChangeTimer++;

				float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)S_SCENE_CHANGE_TIME);
				screenColor.x = color;
				screenColor.y = color;
				screenColor.z = color;

				//	色設定
				mainScene->SetColor(screenColor);
			}

			if (sceneChangeTimer >= S_SCENE_CHANGE_TIME) {
				//	フェードアウト済みロード画面へ
				endLoading = false;
				//	非同期
				sceneInitInfo = std::async(std::launch::async, [this] {return SceneChange(); });
				sceneChangeTimer = S_SCENE_CHANGE_TIME;
			}
		}
	}
	else {
		std::future_status loadStatus = sceneInitInfo.wait_for(std::chrono::seconds(0));
		if (loadStatus == std::future_status::ready) {
			//	ロード終わりフラグ
			endLoading = true;
			loadObj->SetIsLoading(!endLoading);
		}
		//	ロード画面
		//float rot = loadSprite->GetRotation();
		//loadSprite->SetRotation(rot + 0.1f);
		//loadSprite->Update();
	}

	loadObj->Update();

	ImGuiManager::GetInstance()->Begin();
	ImGuiManager::GetInstance()->End();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	dx->PrevPostEffect(shadowEffect.get());

	if (endLoading) {
		scene->DrawShadow();
	}
	
	dx->PostEffectDraw(shadowEffect.get());

	dx->PrevPostEffect(xbulr.get());

	shadowEffect->Draw(true, false, true);

	dx->PostEffectDraw(xbulr.get());

	dx->PrevPostEffect(ybulr.get());

	xbulr->Draw(false, true, true);

	dx->PostEffectDraw(ybulr.get());
	
	dx->PrevPostEffect(mainScene.get());

	if (endLoading) {
		scene->Draw();
	}

	dx->PostEffectDraw(mainScene.get());


	FLOAT luminnceClearColor_[] = { 0.0f,0.0f,0.0f,1.0f };
	dx->PrevPostEffect(luminnce.get(), luminnceClearColor_);

	mainScene->DrawLuminnce();

	dx->PostEffectDraw(luminnce.get());


	dx->PrevPostEffect(xbulrluminnce.get());

	luminnce->Draw(true, false, false);

	dx->PostEffectDraw(xbulrluminnce.get());

	dx->PrevPostEffect(ybulrluminnce.get());

	xbulrluminnce->Draw(false, true, false);

	dx->PostEffectDraw(ybulrluminnce.get());
#pragma endregion

#pragma region MultiPath
	dx->PrevDraw();

	//ybulr->Draw(false, false);
	mainScene->Draw(false, false, false, ybulrluminnce->GetTexture().GetHandle());
	if (!endLoading) {
		//	ロード画面
		//loadSprite->Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
	}
	loadObj->Draw();

	ImGuiManager::GetInstance()->Draw();
	dx->PostDraw();
#pragma endregion
}

void SceneManager::SceneChange()
{
	if (nextScene) {
		if (scene) {
			scene->Finalize();
			delete scene;
		}

		scene = nextScene;
		scene->Initialize();
		//	画像転送
		MyDirectX::GetInstance()->UploadTexture();
		sceneInitialized = true;
		nextScene = nullptr;
	}
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene = sceneFactry->CreateScene(sceneName);
	if (nextScene) sceneInitialized = false;
}
