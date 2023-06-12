#pragma once
#include <wrl.h>
#include <d3d12.h>

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
};

