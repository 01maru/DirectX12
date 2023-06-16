#pragma once
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include "Vector2D.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class InputKeyboard
{
public:
	enum MouseButton {
		LeftClick = 0,
		RightClick,
		WheelClick,
	};
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	BYTE key[256] = {};
	BYTE prev[256] = {};
	ComPtr<IDirectInputDevice8> keyboard = nullptr;

public:
	void Initialize(IDirectInput8* directInput);
	void Update();

	bool GetKey(int _key);
	bool GetTrigger(int _key);
	bool ReleaseKey(int _key);
};

