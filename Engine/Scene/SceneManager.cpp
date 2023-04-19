#include "SceneManager.h"
#include "Easing.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

const int SceneManager::SCENE_CHANGE_TIME = 60;

SceneManager::~SceneManager()
{
	scene->Finalize();
	delete scene;
	ImGuiManager::DeleteInstance();
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager* instance = new SceneManager;
	return instance;
}

void SceneManager::DeleteInstance()
{
	delete SceneManager::GetInstance();
}

void SceneManager::Initialize()
{
	shadowEffect = std::make_unique<PostEffect>();
	shadowEffect->Initialize();
	shadowEffect2 = std::make_unique<PostEffect>();
	shadowEffect2->Initialize(DXGI_FORMAT_R32_FLOAT);

	sceneFactry = std::make_unique<SceneFactory>();
	scene = sceneFactry->CreateScene("GAMESCENE");
	scene->Initialize();
	endLoading = true;

	loadTex = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/loading.png");
	loadSprite = std::make_unique<Sprite>(loadTex);
	loadSprite->SetPosition(Vector2D{ Window::window_width - 96,Window::window_height - 98 });
	loadSprite->SetAnchorPoint(Vector2D{ 0.5,0.5 });
	loadSprite->SetSize(Vector2D{ 64,64 });
	loadSprite->TransferVertex();

	ImGuiManager::GetInstance()->Initialize();

	//	画像転送
	MyDirectX::GetInstance()->UploadTexture();
}

void SceneManager::Update()
{
	if (endLoading) {
		if (sceneInitialized) {

			//	フェードイン
			if (sceneChangeTimer > 0) {
				sceneChangeTimer--;

				float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)SCENE_CHANGE_TIME);
				screenColor.x = color;
				screenColor.y = color;
				screenColor.z = color;

				//	色設定
				shadowEffect->SetColor(screenColor);

				scene->MatUpdate();
			}
			else {
				//	同期処理
				scene->Update();
			}
		}
		else {
			//	nextSceneがセットされたら
			//	フェードアウト
			if (sceneChangeTimer < SCENE_CHANGE_TIME) {
				sceneChangeTimer++;

				float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)SCENE_CHANGE_TIME);
				screenColor.x = color;
				screenColor.y = color;
				screenColor.z = color;

				//	色設定
				shadowEffect->SetColor(screenColor);
			}

			if (sceneChangeTimer >= SCENE_CHANGE_TIME) {
				//	フェードアウト済みロード画面へ
				endLoading = false;
				//	非同期
				sceneInitInfo = std::async(std::launch::async, [this] {return SceneChange(); });
				sceneChangeTimer = SCENE_CHANGE_TIME;
			}
		}
	}
	else {
		std::future_status loadStatus = sceneInitInfo.wait_for(std::chrono::seconds(0));
		if (loadStatus == std::future_status::ready) {
			//	ロード終わりフラグ
			endLoading = true;
		}
		//	ロード画面
		float rot = loadSprite->GetRotation();
		loadSprite->SetRotation(rot + 0.1f);
		loadSprite->MatUpdate();
	}

	ImGuiManager::GetInstance()->Begin();
	ImGuiManager::GetInstance()->End();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	dx->PrevPostEffect(shadowEffect2.get());

	if (endLoading) {
		scene->DrawShadow();
	}
	
	dx->PostEffectDraw(shadowEffect2.get());

	dx->PrevPostEffect(shadowEffect.get());

	if (endLoading) {
		scene->Draw();
	}

	dx->PostEffectDraw(shadowEffect.get());
#pragma endregion

#pragma region MultiPath
	dx->PrevDraw();

	shadowEffect->Draw();
	if (!endLoading) {
		//	ロード画面
		loadSprite->Draw();
	}

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
