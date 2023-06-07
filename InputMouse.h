#pragma once
#include <dinput.h>
#include <wrl.h>
#include "Vector2D.h"

class InputMouse
{
public:
	enum MouseButton {
		LeftClick = 0,
		RightClick,
		WheelClick,
	};
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	DIMOUSESTATE click_ = {};
	DIMOUSESTATE prevClick_ = {};

	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> mouse_;
	Vector2D cursor_;
	Vector2D prevCursor_;
	bool isRockCursor_ = false;

	//	ì¸óÕÇÃèÓïÒê›íË
	void SetInputInfo();
	void RockCursor();
public:
	void Initialize();
	void Update();

	bool Click(int type);
	bool ClickTrigger(int type);

	Vector2D GetCursor() { return cursor_; }
	Vector2D GetPrevCursor() { return prevCursor_; }
	void RockCursor(bool isRock) { isRockCursor_ = isRock; }
	LONG Wheel();
};

