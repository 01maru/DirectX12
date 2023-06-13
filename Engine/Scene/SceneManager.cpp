#include "SceneManager.h"
#include "Easing.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Window.h"
#include "PipelineManager.h"

#include "LoadingSprite.h"
#include "SceneFactory.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::FirstSceneInitialize()
{
	scene_->Initialize();
	//	画像転送
	MyDirectX::GetInstance()->UploadTexture();
}

void SceneManager::Initialize()
{
	sceneFactry_ = std::make_unique<SceneFactory>();
	scene_.reset(sceneFactry_->CreateScene("TITLESCENE"));

#pragma region Loading

	endLoading = true;
	loadObj = std::make_unique<LoadingSprite>();
	loadObj->Initialize();
	loadObj->SetIsLoading(!endLoading);

#pragma endregion

#pragma region PostEffect

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

#pragma endregion

	sceneChangeCounter.Initialize(60, true, true);

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Initialize();
#endif // _DEBUG

#pragma region SplashScreen

#ifdef NDEBUG

	//	非同期
	sceneInitInfo = std::async(std::launch::async, [this] {return FirstSceneInitialize(); });
	isSplashScreen = true;

	rogoUI = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/rogo.png");
	splashSprite = std::make_unique<Sprite>();
	splashSprite->Initialize(rogoUI);
	splashSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,Window::window_height / 2.0f });
	splashSprite->SetAnchorPoint(Vector2D{ 0.5f,0.5f });

	//	画像転送
	MyDirectX::GetInstance()->UploadTexture();

#endif // NDEBUG

#ifdef _DEBUG
	
	FirstSceneInitialize();

#endif // _DEBUG


#pragma endregion
}

void SceneManager::Finalize()
{
	scene_->Finalize();
	ImGuiManager::GetInstance()->Finalize();
}

void SceneManager::Update()
{
	ScreenColorUpdate();

#ifdef NDEBUG

	SplashUpdate();

#endif // NDEBUG

	SceneUpdate();

	loadObj->Update();

#ifdef _DEBUG

	ImguiUpdate();

#endif // _DEBUG
}

void SceneManager::ScreenColorUpdate()
{
	//	スプラッシュスクリーンだったら
	if (isSplashScreen) return;

	sceneChangeCounter.Update();

	if (sceneChangeCounter.GetIsActive()) {
		float color = Easing::lerp(1.0f, 0.0f, sceneChangeCounter.GetCountPerMaxCount());
		screenColor.x = color;
		screenColor.y = color;
		screenColor.z = color;

		//	色設定
		mainScene->SetColor(screenColor);
	}
}

void SceneManager::SplashUpdate()
{
	static bool easeInSplashScreen = true;
	static int splashCount = 0;

	//	スプラッシュスクリーンじゃなかったら
	if (!isSplashScreen) return;
	

	float color = 0.0f;
	if (splashCount == 2) color = 1.0f;

	else if (splashCount == 0 || splashCount == 4) color = 0.0f;

	else color = Easing::lerp(0.0f, 1.0f, (float)sceneChangeTimer / S_SCENE_CHANGE_TIME);

	splashSprite->SetColor({ 1.0f,1.0f,1.0f,color });
	splashSprite->Update();

	std::future_status loadStatus = sceneInitInfo.wait_for(std::chrono::seconds(0));
	if (loadStatus == std::future_status::ready && !easeInSplashScreen) {
		if (sceneChangeTimer > 0) {
			sceneChangeTimer--;
		}
		else {
			splashCount++;
			sceneChangeTimer = S_SCENE_CHANGE_TIME;

			//	スプラッシュスクリーン終わりフラグ
			if (splashCount == 5) {
				isSplashScreen = false;
				scene_->Update();

				sceneChangeCounter.SetIsIncrement(false);
				sceneChangeCounter.StartCount();


				screenColor = { 0.0f,0.0f,0.0f,1.0f };
				//	色設定
				mainScene->SetColor(screenColor);
			}
		}
	}
	else {
		if (sceneChangeTimer < S_SCENE_CHANGE_TIME) {
			sceneChangeTimer++;
		}
		else {
			if (splashCount != 1) {
				sceneChangeTimer = 0;
			}
			else {
				easeInSplashScreen = false;
			}
			splashCount++;
		}
	}
}

void SceneManager::SceneFadeInUpdate()
{
	if (!endLoading) return;
	if (!sceneInitialized) return;

	//	フェードイン済み
	bool fadedIn = sceneChangeCounter.GetFrameCount() == 0;
	if (fadedIn) {
		//	同期処理
		scene_->Update();
	}
}

void SceneManager::SceneFadeOutUpdate()
{
	if (!endLoading) return;
	if (sceneInitialized) return;

	//	フェードアウト済み
	bool fadedOut = sceneChangeCounter.GetFrameCount() == sceneChangeCounter.GetMaxFrameCount();
	if (fadedOut) {
		//	フェードアウト済みロード画面へ
		endLoading = false;
		//	非同期
		sceneInitInfo = std::async(std::launch::async, [this] {return SceneChange(); });
	}
}

void SceneManager::SceneAsyncUpdate()
{
	//	ロード中じゃなかったら
	if (endLoading) return;

	std::future_status loadStatus = sceneInitInfo.wait_for(std::chrono::seconds(0));
	if (loadStatus == std::future_status::ready) {
		//	ロード終わりフラグ
		endLoading = true;
		loadObj->SetIsLoading(!endLoading);
		//	フェードイン
		sceneChangeCounter.SetIsIncrement(false);
		sceneChangeCounter.StartCount();
	}
}

void SceneManager::SceneUpdate()
{
	//	スプラッシュスクリーンだったら
	if (isSplashScreen) return;

	SceneFadeInUpdate();
	SceneFadeOutUpdate();
	
	SceneAsyncUpdate();
}

void SceneManager::ImguiUpdate()
{
	ImGuiManager::GetInstance()->Begin();
	if (endLoading) {
		scene_->ImguiUpdate();
	}
	ImGuiManager::GetInstance()->End();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	FLOAT shadowClearColor_[] = { 1.0f,1.0f,1.0f,1.0f };

	dx->PrevPostEffect(shadowEffect.get(), shadowClearColor_);

	if (endLoading && !isSplashScreen) {
		scene_->DrawShadow();
	}
	
	dx->PostEffectDraw(shadowEffect.get());

	dx->PrevPostEffect(xbulr.get(), shadowClearColor_);

	shadowEffect->Draw(true, false, true);

	dx->PostEffectDraw(xbulr.get());

	dx->PrevPostEffect(ybulr.get(), shadowClearColor_);

	xbulr->Draw(false, true, true);

	dx->PostEffectDraw(ybulr.get());
	
	dx->PrevPostEffect(mainScene.get());

	if (endLoading && !isSplashScreen) {
		scene_->Draw();
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
	FLOAT clearColor_[] = { 0.0f,0.0f,0.0f,1.0f };
	dx->PrevDraw(clearColor_);

	if (!isSplashScreen) {
		mainScene->Draw(false, false, false, ybulrluminnce->GetTexture().GetHandle());
	}
	
	loadObj->Draw();

#ifdef NDEBUG

	if (isSplashScreen) {
		splashSprite->Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
	}

#endif // NDEBUG

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Draw();
#endif // _DEBUG
	dx->PostDraw();
#pragma endregion
}

void SceneManager::SceneChange()
{
	if (nextScene_ != nullptr) {
		if (scene_ != nullptr) {
			scene_->Finalize();
		}

		scene_.reset(nextScene_.get());
		scene_->Initialize();
		//	画像転送
		MyDirectX::GetInstance()->UploadTexture();
		sceneInitialized = true;
		nextScene_.release();
	}
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene_.reset(sceneFactry_->CreateScene(sceneName));
	
	//	nextSceneがセットされたら
	if (nextScene_ != nullptr) {
		sceneInitialized = false;
		loadObj->SetIsLoading(true);

		sceneChangeCounter.SetIsIncrement(true);
		sceneChangeCounter.StartCount();
	}
}
