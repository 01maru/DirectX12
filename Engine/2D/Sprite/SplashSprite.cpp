#include "SplashSprite.h"
#include "TextureManager.h"
#include "Window.h"
#include "PipelineManager.h"
#include "Easing.h"

void SplashSprite::Initialize()
{
	count_ = std::make_unique<FrameCounter>();
	count_->Initialize(30, true, true, 4);

	rogoTex_ = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/rogo.png");

	splashSprite_ = std::make_unique<Sprite>();
	splashSprite_->Initialize(rogoTex_);
	splashSprite_->SetPosition(Vector2D{ Window::window_width / 2.0f,Window::window_height / 2.0f });
	splashSprite_->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
}

void SplashSprite::Finalize()
{
}

void SplashSprite::Update()
{
	if (!count_->GetIsActive()) return;

	count_->Update();
	
	float color = 0.0f;
	if (count_->GetCount() == 1 || count_->GetCount() == 2) color = 1.0f;

	else color = Easing::lerp(0.0f, 1.0f, count_->GetCountPerMaxCount());

	splashSprite_->SetColor({ 1.0f,1.0f,1.0f,color });
	splashSprite_->Update();
}

void SplashSprite::Draw()
{
	if (!count_->GetIsActive()) return;

	splashSprite_->Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
}
