#include "SceneFactory.h"

#pragma region Scene��ǉ����邽�тɂ���Scene��include
#include "GameScene.h"
#include "TitleScene.h"
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
