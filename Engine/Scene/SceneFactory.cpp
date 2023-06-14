#include "SceneFactory.h"

#pragma region Scene��ǉ����邽�тɂ���Scene��include
#include "TitleScene.h"
#include "GameScene.h"
#pragma endregion

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    std::unique_ptr<IScene> newScene;

    if (sceneName == "TITLESCENE") {
        newScene = std::make_unique<TitleScene>();
    }
    else if(sceneName == "GAMESCENE") {
        newScene = std::make_unique<GameScene>();
    }

    return std::move(newScene);
}
