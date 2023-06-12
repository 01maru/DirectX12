#include "SceneFactory.h"

#pragma region Sceneを追加するたびにそのSceneをinclude
#include "TitleScene.h"
#include "GameScene.h"
#pragma endregion

IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    IScene* newScene = nullptr;

    if (sceneName == "TITLESCENE") {
        newScene = new TitleScene();
    }
    else if(sceneName == "GAMESCENE") {
        newScene = new GameScene();
    }

    return newScene;
}
