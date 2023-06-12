#pragma once
#include <string>

class IScene;

class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;
	virtual IScene* CreateScene(const std::string& sceneName) = 0;
};

