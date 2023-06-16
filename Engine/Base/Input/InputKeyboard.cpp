#include "InputKeyboard.h"
#include "Window.h"

void InputKeyboard::Initialize(IDirectInput8* directInput)
{
	Window* win = Window::GetInstance();
	HRESULT result;

	//	デバイス生成(キーボード以外も可能)
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = keyboard->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void InputKeyboard::Update()
{
	//	前フレームの情報取得
	for (size_t i = 0; i < sizeof(key); i++)
	{
		prev[i] = key[i];
	}

	//	キー情報取得
	keyboard->Acquire();
	//	全キーの入力情報取得
	keyboard->GetDeviceState(sizeof(key), key);
}

bool InputKeyboard::GetKey(int _key)
{
	return key[_key];
}

bool InputKeyboard::GetTrigger(int _key)
{
	return key[_key] && !prev[_key];
}

bool InputKeyboard::ReleaseKey(int _key)
{
	return prev[_key] && !key[_key];
}
