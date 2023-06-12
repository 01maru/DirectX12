#include "SceneFactory.h"

#pragma region Scene‚ð’Ç‰Á‚·‚é‚½‚Ñ‚É‚»‚ÌScene‚ðinclude
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
