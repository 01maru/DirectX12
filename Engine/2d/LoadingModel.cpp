#include "LoadingModel.h"
#include "TextureManager.h"
#include "Window.h"
#include "Easing.h"

#include "ObjModel.h"
#include "ObjCamera2D.h"

void LoadingModel::Initialize()
{
	loadModel = std::make_unique<ObjModel>("player");

	loadObj.reset(Object3D::Create(loadModel.get()));
	loadObj->SetColor({ 1.0f,1.0f,1.0f });
	loadObj->SetScale({ 50.0f,50.0f,50.0f });
	loadObj->SetPosition({ Window::window_width / 2.0f - 100.0f,-Window::window_height / 2.0f + 50.0f,0.0f });

	camera = std::make_unique<ObjCamera2D>();
}

void LoadingModel::Update()
{
    if (!loading_ && !fadeIn_) {
        //  ローディング中じゃなくfadeIn済みだったら
        if (easeCount_ > 0) {
            easeCount_--;

            float alphaColor = Easing::EaseOut(1.0f, 0.0f, 1.0f - (float)easeCount_ / sEaseMaxCount, 4);

            loadObj->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
        }
    }
    else {
        bool isEasing = easeCount_ < sEaseMaxCount;

        if (isEasing) {
            easeCount_++;

            float alphaColor = Easing::EaseOut(0.0f, 1.0f, (float)easeCount_ / sEaseMaxCount, 4);

            loadObj->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
        }

        //  透過値1.0になったらfalseに
        if (!isEasing)  fadeIn_ = false;

        camera->MatUpdate();
    }

    //  モデル表示中は回転させる
    if (easeCount_ > 0) {
        Vector3D rot = loadObj->GetRotation();
        rot.y -= 0.1f;
        loadObj->SetRotation(rot);
        loadObj->MatUpdate(camera.get());
    }
}

void LoadingModel::Draw()
{
    if (easeCount_ > 0) {
        loadObj->DrawSilhouette();
    }
}
