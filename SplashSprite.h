#pragma once
#include "Sprite.h"
#include "FrameCounter.h"

class SplashSprite
{
private:
	Texture rogoTex_;
	std::unique_ptr<Sprite> splashSprite_;
	std::unique_ptr<FrameCounter> count_;
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	bool SplashEnd() { return count_->GetCount() == count_->GetMaxCount(); }
};

