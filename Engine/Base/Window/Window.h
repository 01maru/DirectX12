#pragma once
#define NOMINMAX
#include <Windows.h>

class Window
{
public:
	// ウィンドウサイズ
	static const int sWIN_WIDTH = 1280; // 横幅
	static const int sWIN_HEIGHT = 720; // 縦幅
private:
	// ウィンドウクラスの設定
	WNDCLASSEX w_{};
	// ウィンドウオブジェクトの生成
	HWND hwnd_;
	// メッセージ
	MSG msg_{};

	Window() {};
	~Window() {};
public:
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static Window* GetInstance();
	Window(const Window& obj) = delete;
	Window& operator=(const Window& obj) = delete;

	void Initialize();
	void Finalize();
	bool MsgUpdate();
	bool EndLoop();

	//	getter
	const HWND& GetHwnd() const { return hwnd_; }
	const WNDCLASSEX& GetWND() const { return w_;}
};

