#include "LoadingSprite.h"
#include "TextureManager.h"
#include "Window.h"
#include "PipelineManager.h"
#include "Easing.h"

void LoadingSprite::Initialize()
{
	loadTex_ = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/loading.png");

	loadSprite_ = std::make_unique<Sprite>();
	loadSprite_->Initialize(loadTex_);
	loadSprite_->SetPosition(Vector2D{ Window::sWIN_WIDTH - 96,Window::sWIN_HEIGHT - 98 });
	loadSprite_->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	loadSprite_->SetSize(Vector2D{ 64,64 });
}

void LoadingSprite::Update()
{
    //  ローディング中じゃなくfadeIn済みだったら
    if (!loading_ && !fadeIn_) {
        if (easeCount_ > 0) {
            easeCount_--;

            float alphaColor = Easing::EaseIn(1.0f, 0.0f, 1.0f - (float)easeCount_ / sEASE_MAX_COUNT, 4);

            loadSprite_->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
        }
    }
    else {
        bool isEasing = easeCount_ < sEASE_MAX_COUNT;

        if (isEasing) {
            easeCount_++;

            float alphaColor = Easing::EaseIn(0.0f, 1.0f, (float)easeCount_ / sEASE_MAX_COUNT, 4);

            loadSprite_->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
        }

        //  透過値1.0になったらfalseに
        if (!isEasing)  fadeIn_ = false;

    }

    if (easeCount_ > 0) {
        float rot = loadSprite_->GetRotation();
        loadSprite_->SetRotation(rot - 0.1f);
        loadSprite_->Update();
    }
}

void LoadingSprite::Draw()
{
    if (easeCount_ > 0) {
        loadSprite_->Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
    }
}
