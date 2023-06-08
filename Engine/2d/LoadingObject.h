#pragma once
#include "ICamera.h"
#include "Object3D.h"
#include "ILoadingObj.h"

#include <memory>

class LoadingObject :public ILoadingObj
{
private:

	std::unique_ptr<ICamera> camera;
	std::unique_ptr<Object3D> loadObj;
	std::unique_ptr<IModel> loadModel;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

