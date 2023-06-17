#pragma once
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")

class Shader
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3DBlob> vsBlob;		// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> hsBlob;		// ハルシェーダオブジェクト
	ComPtr<ID3DBlob> dsBlob;		// ドメインシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;		// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;		// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;		// エラーオブジェクト

private:
	void Error(HRESULT result);

public:
	Shader() {};
	Shader(const LPCWSTR& VSFileName, const LPCWSTR& PSFileName, const LPCSTR& pEntryPoint = "main"
		, const LPCWSTR& GSFileName = nullptr, const LPCWSTR& DSFileName = nullptr, const LPCWSTR& HSFileName = nullptr);
	void Initialize(const LPCWSTR& VSFileName, const LPCWSTR& PSFileName, const LPCSTR& pEntryPoint = "main"
		, const LPCWSTR& GSFileName = nullptr, const LPCWSTR& DSFileName = nullptr, const LPCWSTR& HSFileName = nullptr);

	//	Getter
	ID3DBlob* VSBlob() { return vsBlob.Get(); }
	ID3DBlob* HSBlob() { return hsBlob.Get(); }
	ID3DBlob* DSBlob() { return dsBlob.Get(); }
	ID3DBlob* GSBlob() { return gsBlob.Get(); }
	ID3DBlob* PSBlob() { return psBlob.Get(); }
};
