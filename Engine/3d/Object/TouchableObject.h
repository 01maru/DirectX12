#pragma once
#include "Object3d.h"

class TouchableObject : public Object3D
{
public:
	static TouchableObject* Create(IModel* model = nullptr);
	bool Initialize(IModel* model_);
};

