#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>

#include "Vector2D.h"

class ImGuiManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> srvHeap;

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

	void SetWindow(const std::string& windowName);
	void EndWindow();
	bool SetButton(const std::string& buttonName, const Vector2D& size = Vector2D());
	void SetSliderFloat(const std::string& sliderName, float& value, float minValue, float maxValue);
	void CheckBox(const std::string& name, bool& flag);
};

