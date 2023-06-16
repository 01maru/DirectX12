#pragma once
#include "InputMouse.h"
#include "InputJoypad.h"
#include <memory>

class InputManager
{
private:
	bool activeMouseImGui_ = false;
	bool activePadImGui_ = false;

	std::unique_ptr<InputMouse> mouse_;
	std::unique_ptr<InputJoypad> joypad_;

private:
	InputManager() {};
	~InputManager() {};
public:
	static InputManager* GetInstance();
	InputManager(const InputManager& obj) = delete;
	InputManager& operator=(const InputManager& obj) = delete;

	void Initialize();
	void Update();
	void ImGuiUpdate();

	//	Getter
	const InputMouse* GetMouse() { return mouse_.get(); }
	const InputJoypad* GetPad() { return joypad_.get(); }
};

