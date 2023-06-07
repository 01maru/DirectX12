#pragma once

class InputManager
{
	InputManager() {};
	~InputManager() {};
public:
	static InputManager* GetInstance();
	InputManager(const InputManager& obj) = delete;
	InputManager& operator=(const InputManager& obj) = delete;

	void Initialize();
	void Update();
};

