#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>

#include "Vector2D.h"

class ImGuiManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

private:	//	関数
	ImGuiManager() {};
	~ImGuiManager() {};
public:
	static ImGuiManager* GetInstance();
	//	コピーコンストラクタ無効
	ImGuiManager(const ImGuiManager& obj) = delete;
	//	代入演算子無効
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;

	void Initialize();
	void Finalize();
	
	//	更新前処理
	void Begin();
	//	更新後処理
	void End();
	//	描画処理
	void Draw();

#pragma region Window

	void BeginWindow(const std::string& windowName);
	void BeginWindow(const std::string& windowName, bool menu);
	void EndWindow();

#pragma endregion

#pragma region Menu

	//	return isActive;
	bool BeginMenuBar();
	void EndMenuBar();

	//	return isActive;
	bool BeginMenu(const std::string& menuName);
	void EndMenu();

	//	MenuButton
	bool MenuItem(const std::string& name);

#pragma endregion

#pragma region Input

	void SameLine();
	bool SetButton(const std::string& buttonName, const Vector2D& size = Vector2D());
	void SetRadioButton(const std::string& buttonName, bool& flag);
	void SetSliderFloat(const std::string& sliderName, float& value, float spd, float minValue, float maxValue);
	void CheckBox(const std::string& name, bool& flag);

#pragma endregion

	void LabelText(const std::string& name, const std::string& text, float value);
	void Text(const char* fmt, ...);

#pragma region Scroll

	//	return isActive;
	bool BeginChild(const Vector2D& size = Vector2D());
	void EndChild();

#pragma endregion
};

