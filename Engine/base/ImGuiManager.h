#pragma once
#include "DirectX.h"

class ImGuiManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> srvHeap;
public:
	ImGuiManager() {};
	~ImGuiManager() {};

	static ImGuiManager* GetInstance();
	static void DeleteInstance();
	//	コピーコンストラクタ無効
	ImGuiManager(const ImGuiManager& obj) = delete;
	//	代入演算子無効
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;

	void Initialize();

	void Begin();
	void End();
	void Draw();
};

