#pragma once
#include "ILoadingObj.h"
#include "Sprite.h"
#include <memory>

class LoadingSprite :public ILoadingObj
{
private:
	Texture loadTex_;
	std::unique_ptr<Sprite> loadSprite_;
	
public:
	LoadingSprite() {}
	~LoadingSprite() override {}
	void Initialize() override;
	void Update() override;
	void Draw() override;
};
