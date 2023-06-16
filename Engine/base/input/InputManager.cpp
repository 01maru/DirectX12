#include "InputManager.h"

#include "ImGuiManager.h"
#include "ImGuiController.h"

InputManager* InputManager::GetInstance()
{
	static InputManager instance;
	return &instance;
}

void InputManager::Initialize()
{
	mouse_ = std::make_unique<InputMouse>();
	mouse_->Initialize();

	joypad_ = std::make_unique<InputJoypad>();
}

void InputManager::Update()
{
	mouse_->Update();

	joypad_->Update();
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
