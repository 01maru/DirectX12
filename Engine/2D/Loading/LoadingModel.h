#pragma once
#include "ICamera.h"
#include "Object3D.h"
#include "ILoadingObj.h"

#include <memory>

class LoadingModel :public ILoadingObj
{
private:
	std::unique_ptr<ICamera> camera_;
	std::unique_ptr<Object3D> loadObj_;
	std::unique_ptr<IModel> loadModel_;

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

