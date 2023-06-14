#pragma once
#include <memory>
#include <string>

class IScene;

class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;
	virtual std::unique_ptr<IScene> CreateScene(const std::string& sceneName) = 0;
};

