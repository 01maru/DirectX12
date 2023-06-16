#pragma once
#include "InputMouse.h"
#include "InputJoypad.h"
#include "InputKeyboard.h"
#include <memory>

class InputManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	bool activeMouseImGui_ = false;
	bool activePadImGui_ = false;

	ComPtr<IDirectInput8> directInput_ = nullptr;

	std::unique_ptr<InputMouse> mouse_;
	std::unique_ptr<InputJoypad> joypad_;
	std::unique_ptr<InputKeyboard> keyboard_;

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
	InputMouse* GetMouse() { return mouse_.get(); }
	InputJoypad* GetPad() { return joypad_.get(); }
	InputKeyboard* GetKeyboard() { return keyboard_.get(); }
};

