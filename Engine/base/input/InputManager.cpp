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
}

void InputManager::Update()
{
	mouse_->Update();
}

void InputManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveInputManager()) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("InputManager");

	imgui->CheckBox("Mouse", activeMouseImGui_);
	if (activeMouseImGui_)	mouse_->ImGuiUpdate();

	imgui->EndWindow();
}
