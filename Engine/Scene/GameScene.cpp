#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "SceneManager.h"
#include "MyXAudio.h"
#include "NormalCamera.h"
#include "TextureManager.h"
#include "FbxModel.h"
#include "ObjModel.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "SceneManager.h"

#include "Input.h"

void GameScene::CollisionUpdate()
{
	//player->CollisionUpdate();
	collisionMan->CheckAllCollisions();
}

void GameScene::MatUpdate()
{
	ParticleManager::GetInstance()->MatUpdate();
	ground->MatUpdate();
	skydome->MatUpdate();
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//delete camera;
}

void GameScene::Initialize()
{
	collisionMan = CollisionManager::GetInstance();

	camera = new MyDebugCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	//camera = Light::GetInstance()->GetDirLightCamera(0);

	Object3D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Model", GPipeline::ALPHA_BLEND));
	Object3D::SetCamera(camera);
	LoadResources();
	
	Particle::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Particle", GPipeline::ADD_BLEND));
	Particle::SetCamera(camera);

	Object2D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Obj2D", GPipeline::ADD_BLEND));
	Object2D::SetCamera(camera);
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
}

void GameScene::Finalize()
{
	MyXAudio::GetInstance()->StopAllLoopSound();
}

void GameScene::LoadResources()
{
#pragma region Model
	modelSkydome = std::make_unique<ObjModel>("skydome");
	modelGround = std::make_unique<ObjModel>("ground");
#pragma endregion
	//	�V��
	skydome.reset(Object3D::Create(modelSkydome.get()));
	//	�n��
	ground.reset(Object3D::Create(modelGround.get()));
#pragma region Texture
	VolumeLightObj::SetLightGraph(TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/lightTex.jpg"));
	reimuG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/reimu.png");
	grassG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/grass.png");
#pragma endregion

#pragma region Sprite
	sprite = std::make_unique<Sprite>(SceneManager::GetInstance()->GetShadowMap());
	sprite->SetSize(Vector2D(200.0f, 200.0f));
#pragma endregion
}

void GameScene::Update()
{
#pragma region �X�V����
	if (Input::GetInstance()->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}

	camera->Update();

	sprite->Update();
	ParticleManager::GetInstance()->Update();

	//DebugTextManager::GetInstance()->Print("test", { 0,Window::window_height/2.0f }, 5);
#pragma endregion
	MatUpdate();
	CollisionUpdate();

	Light::GetInstance()->SetDirLightColor(0, Vector3D(1.0f, 1.0f, 1.0f));
}

void GameScene::DrawShadow()
{
}

void GameScene::Draw()
{
	//	�V��
	skydome->Draw();
	//	�n��
	ground->Draw();

	//DebugTextManager::GetInstance()->Draw();
	ParticleManager::GetInstance()->Draw();
}
