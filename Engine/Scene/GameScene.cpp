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

void GameScene::CollisionUpdate()
{
	//player->CollisionUpdate();
	collisionMan->CheckAllCollisions();
}

void GameScene::MatUpdate()
{
	ground->MatUpdate();
	hill->MatUpdate();
	skydome->MatUpdate();

	player->MatUpdate();
	tree->MatUpdate();
	tree->PlayAnimation();
	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->MatUpdate();
	}
	//sphere2->MatUpdate();

	for (int i = 0; i < grass.size(); i++)
	{
		grass[i].MatUpdate();
	}

	for (int i = 0; i < testVolLight.size(); i++)
	{
		testVolLight[i].MatUpdate();
	}
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;
	for (int i = 0; i < tree2.size(); i++)
	{
		delete tree2[i];
	}
}

void GameScene::Initialize()
{
	collisionMan = CollisionManager::GetInstance();

	camera = new MyDebugCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	Object3D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Model", GPipeline::ALPHA_BLEND));
	Object3D::SetCamera(camera);
	LoadResources();

	Grass::SetPipeline(PipelineManager::GetInstance()->GetPipeline("GrassParticle", GPipeline::ALPHA_BLEND));
	Grass::SetCamera(camera);

	Object2D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Obj2D", GPipeline::ADD_BLEND));
	Object2D::SetCamera(camera);
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
	player->PlayerInitialize(modelSword.get());
	float size = 0.05f;
	player->SetScale({ size,size,size });
	player->SetRotation({ 0.0f,MyMath::PI,0.0f });

	grass.push_back(Grass());
	float dis = 6.0f;
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 0.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 0.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 0.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f,dis), 0.0f, 0.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f,dis), 0.0f, 0.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f,dis), 0.0f, 0.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, 2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, 2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, 2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -2.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -3.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -3.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -3.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -3.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -3.0f)));
	grass.push_back(Grass(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -3.0f)));

	testVolLight.push_back(VolumeLightObj(Vector2D(5.0f, 25.0f), Vector3D(0.0f, 0.0f, -2.0f), 30.0f));

	mord = Phong;
}

void GameScene::Finalize()
{
	MyXAudio::GetInstance()->StopAllLoopSound();
}

void GameScene::LoadResources()
{
#pragma region Model
	modelSword = std::make_unique<FbxModel>("human");

	modelSkydome = std::make_unique<ObjModel>("skydome");
	modelHill = std::make_unique<ObjModel>("ground1");
	modelGround = std::make_unique<ObjModel>("ground");
	//	丘
	modelTree = std::make_unique<FbxModel>("tree4");
	modelTree2 = std::make_unique<ObjModel>("tree2");
#pragma endregion
	//	天球
	skydome.reset(Object3D::Create(modelSkydome.get()));
	//	地面
	ground.reset(TouchableObject::Create(modelGround.get()));
	ground->SetScale(Vector3D(1.0f, 1.0f, 0.1f));
	hill.reset(Object3D::Create(modelHill.get()));
	float groundscale = 10.0f;
	hill->SetScale(Vector3D(groundscale, 1.0f, 1.0f));
	hill->SetPosition(Vector3D(0.0f, 0.0f, 10.0f));

	tree.reset(Object3D::Create(modelTree.get()));
	//float size = 0.01f;
	//tree->SetScale({ size,size,size });
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->SetPosition(Vector3D(-12.0f + 2.0f * i, 0.0f, 5.0f+MyMath::GetRand(0.0f,6.0f)));
	}
#pragma region Texture
	VolumeLightObj::SetLightGraph(TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/lightTex.jpg"));
	reimuG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/reimu.png");
	grassG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/grass.png");
#pragma endregion

#pragma region Sprite
	sprite = std::make_unique<Sprite>(reimuG);
	sprite->SetSize(Vector2D(200.0f, 200.0f));
	//sprite->SetTextureLeftTop(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
	//sprite->SetTextureSize(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
#pragma endregion
}

void GameScene::Update()
{
#pragma region 更新処理

	//	カメラ切り替え
	if (input->GetTrigger(DIK_V)) {
		isDebug = !isDebug;
		if (isDebug) {
			delete camera;
			camera = new MyDebugCamera();
			camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
			Player::SetCamera(camera);
			Grass::SetCamera(camera);
			Object3D::SetCamera(camera);
		}
		else {
			delete camera;
			camera = new NormalCamera();
			camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
			Player::SetCamera(camera);
			Grass::SetCamera(camera);
			Object3D::SetCamera(camera);
		}
	}

	if (input->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}

	camera->Update();

	sprite->Update();

#pragma endregion
	MatUpdate();
	CollisionUpdate();

	Light::GetInstance()->SetFogActive(input->GetKey(DIK_SPACE));

	if (input->GetTrigger(DIK_1)) {
		mord = Phong;

		Light::GetInstance()->SetCircleShadowActive(0, false);
		Light::GetInstance()->SetSpotLightActive(0, true);
		Light::GetInstance()->SetSpotLightActive(1, true);
		Light::GetInstance()->SetSpotLightActive(2, true);
		Light::GetInstance()->SetDirLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(1, false);
		Light::GetInstance()->SetPointLightActive(2, false);
	}
	else if (input->GetTrigger(DIK_2)) {
		mord = PointLight;

		Light::GetInstance()->SetCircleShadowActive(0, false);
		Light::GetInstance()->SetSpotLightActive(0, false);
		Light::GetInstance()->SetDirLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(0, true);
		Light::GetInstance()->SetPointLightActive(1, true);
		Light::GetInstance()->SetPointLightActive(2, true);
	}
	else if (input->GetTrigger(DIK_3)) {
		mord = SpotLight;

		Light::GetInstance()->SetCircleShadowActive(0, false);
		Light::GetInstance()->SetSpotLightActive(0, true);
		Light::GetInstance()->SetDirLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(1, false);
		Light::GetInstance()->SetPointLightActive(2, false);
	}
	else if (input->GetTrigger(DIK_4)) {
		mord = CircleShadow;

		Light::GetInstance()->SetSpotLightActive(0, false);
		Light::GetInstance()->SetCircleShadowActive(0, true);
		Light::GetInstance()->SetDirLightActive(0, true);
		Light::GetInstance()->SetPointLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(1, false);
		Light::GetInstance()->SetPointLightActive(2, false);
	}

	//Vector3D playerGroundPos = player->GetPosition();
	//playerGroundPos.y += 0.1f;
	//Light::GetInstance()->SetCircleShadowCasterPos(0, playerGroundPos);
	Light::GetInstance()->SetCircleShadowDir(0, { 0.0f,1.0f,0.0f });
	Light::GetInstance()->SetCircleShadowFactorAngle(0, { 0.0f,0.5f });
	Light::GetInstance()->SetCircleShadowAtten(0, { 0.2f,0.2f,0.0f });
}

void GameScene::Draw()
{
	//	地面
	ground->Draw();
	hill->Draw();
	//	天球
	skydome->Draw();
	//	木
	tree->Draw();

	//player->Draw();

	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->Draw();
	}
	
	for (int i = 0; i < grass.size(); i++)
	{
		grass[i].Draw(grassG.GetHandle());
	}
	for (int i = 0; i < testVolLight.size(); i++)
	{
		testVolLight[i].Draw();
	}

	//sprite->Draw();
}
