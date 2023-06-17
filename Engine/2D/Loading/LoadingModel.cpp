#include "LoadingModel.h"
#include "Window.h"
#include "Easing.h"

#include "ObjModel.h"
#include "ObjCamera2D.h"

void LoadingModel::Initialize()
{
    loadModel_ = std::make_unique<ObjModel>("player");

	loadObj_.reset(Object3D::Create(loadModel_.get()));
	loadObj_->SetColor({ 1.0f,1.0f,1.0f });
	loadObj_->SetScale({ 50.0f,50.0f,50.0f });
	loadObj_->SetPosition({ Window::sWIN_WIDTH / 2.0f - 100.0f,-Window::sWIN_HEIGHT / 2.0f + 50.0f,0.0f });

    camera_ = std::make_unique<ObjCamera2D>();
}

void LoadingModel::Update()
{
    if (!loading_ && !fadeIn_) {
        //  ローディング中じゃなくfadeIn済みだったら
        if (easeCount_ > 0) {
            easeCount_--;

            float alphaColor = Easing::EaseOut(1.0f, 0.0f, 1.0f - (float)easeCount_ / sEASE_MAX_COUNT, 4);

            loadObj_->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
        }
    }
    else {
        bool isEasing = easeCount_ < sEASE_MAX_COUNT;

        if (isEasing) {
            easeCount_++;

            float alphaColor = Easing::EaseOut(0.0f, 1.0f, (float)easeCount_ / sEASE_MAX_COUNT, 4);

            loadObj_->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
        }

        //  透過値1.0になったらfalseに
        if (!isEasing)  fadeIn_ = false;

        camera_->MatUpdate();
    }

    //  モデル表示中は回転させる
    if (easeCount_ > 0) {
        Vector3D rot = loadObj_->GetRotation();
        rot.y -= 0.1f;
        loadObj_->SetRotation(rot);
        loadObj_->MatUpdate(camera_.get());
    }
}

void LoadingModel::Draw()
{
    if (easeCount_ > 0) {
        loadObj_->DrawSilhouette();
    }
}
