#include "InputManager.h"
#include "Window.h"

#include "ImGuiManager.h"
#include "ImGuiController.h"
#include <cassert>

InputManager* InputManager::GetInstance()
{
	static InputManager instance;
	return &instance;
}

void InputManager::Initialize()
{
	//	DirectInput初期化
	HRESULT result = DirectInput8Create(
		Window::GetInstance()->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	mouse_ = std::make_unique<InputMouse>();
	mouse_->Initialize(directInput_.Get());

	joypad_ = std::make_unique<InputJoypad>();

	keyboard_ = std::make_unique<InputKeyboard>();
	keyboard_->Initialize(directInput_.Get());
}

void InputManager::Update()
{
	mouse_->Update();

	joypad_->Update();

	keyboard_->Update();
}

void InputManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveInputManager()) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("InputManager");

	mouse_->ImGuiUpdate();

	joypad_->ImGuiUpdate();

	imgui->EndWindow();
}
