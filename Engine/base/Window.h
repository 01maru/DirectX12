#pragma once
#define NOMINMAX
#include <Windows.h>

class Window
{
public:
	// ウィンドウサイズ
	static const int window_width = 1280; // 横幅
	static const int window_height = 720; // 縦幅
private:
	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	// ウィンドウオブジェクトの生成
	HWND hwnd;
	// メッセージ
	MSG msg{};

	Window();
	~Window();
public:
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static Window* GetInstance();
	Window(const Window& obj) = delete;
	Window& operator=(const Window& obj) = delete;

	void Initialize();
	bool MsgUpdate();
	bool EndLoop();

	//	getter
	HWND GetHwnd() const { return hwnd; }
	WNDCLASSEX GetWND() const { return w;}
};

