#include "SceneFactory.h"

#pragma region Scene‚ð’Ç‰Á‚·‚é‚½‚Ñ‚É‚»‚ÌScene‚ðinclude
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
