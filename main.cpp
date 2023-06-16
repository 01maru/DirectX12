#include "MyGame.h"
#include "Window.h"
#include <memory>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	std::unique_ptr<Framework> game = std::make_unique<MyGame>();

	game->Run();

	return 0;
}