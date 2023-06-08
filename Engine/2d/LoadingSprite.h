#pragma once
#include "Sprite.h"
#include <memory>
#include "ILoadingObj.h"

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
