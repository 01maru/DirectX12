#pragma once
#include "IScene.h"
#include "IModel.h"
#include "Object3D.h"
#include "Sprite.h"

#include "ICamera.h"
#include "Texture.h"

class GameScene :public IScene
{
private:
	ICamera* camera = nullptr;

#pragma region Obj3D

	std::unique_ptr<Object3D> skydome_;
	std::unique_ptr<Object3D> ground_;
	std::unique_ptr<Object3D> cube_;

#pragma endregion

#pragma region Model

	std::unique_ptr<IModel> modelSkydome_;
	std::unique_ptr<IModel> modelGround_;
	std::unique_ptr<IModel> modelCube_;

#pragma endregion

#pragma region Sprite

	std::unique_ptr<Sprite> sprite_;

#pragma endregion
	
#pragma region Texture
	Texture reimuG;
	Texture grassG;
#pragma endregion

private:	//	関数
	void MatUpdate() override;

public:	
	~GameScene() override {};
	void Initialize() override;
	void LoadResources() override;
	void Finalize() override;
	void Update() override;
	void ImguiUpdate() override;
	void DrawShadow() override;
	void Draw() override;
};

