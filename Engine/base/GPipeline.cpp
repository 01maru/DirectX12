#include "GPipeline.h"
#include <vector>
#include "DirectX.h"
#include "PostEffect.h"

MyDirectX* GPipeline::dx = MyDirectX::GetInstance();

void GPipeline::Update(D3D_PRIMITIVE_TOPOLOGY primitive)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	dx->GetCmdList()->SetPipelineState(state.Get());
	dx->GetCmdList()->IASetPrimitiveTopology(primitive);
}

void GPipeline::Setting()
{
	dx->GetCmdList()->SetGraphicsRootSignature(rootSignature.Get());
}

void GPipeline::Init(Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, int constBuffNum, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord, D3D12_CULL_MODE cullmord, D3D12_DEPTH_WRITE_MASK depth_write_mask, bool isDeep, DXGI_FORMAT format, int textureNum)
{
	HRESULT result;
	// シェーダーの設定
	SetShader(shader);

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

#pragma region Rasterizer
	// 設定
	pipelineDesc.RasterizerState.CullMode = cullmord; // 背面カリング
	pipelineDesc.RasterizerState.FillMode = fillmord; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
#pragma endregion

#pragma region  Blending
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blend(blenddesc);
#pragma endregion

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = inputLayoutSize;

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = topologyType;

	// その他の設定
	pipelineDesc.NumRenderTargets = 2;		// 描画対象
	for (int i = 0; i < 2; i++)
	{
		pipelineDesc.RTVFormats[i] = format; // 0~255指定のRGBA
	}
	pipelineDesc.SampleDesc.Count = 1;							  // 1ピクセルにつき1回サンプリング

	//	デプスステンシルステート設定
	if (isDeep) {
		pipelineDesc.DepthStencilState.DepthEnable = true;								//	深度テストを行う
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//	小さければ合格
	}
	else {
		pipelineDesc.DepthStencilState.DepthEnable = false;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	}
	pipelineDesc.DepthStencilState.DepthWriteMask = depth_write_mask;		//	書き込み許可するかどうか
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//	深度フォーマット

	SetRootSignature(constBuffNum, textureNum);

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	result = dx->GetDev()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&state));
	assert(SUCCEEDED(result));
}

void GPipeline::Blend(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, const int mord)
{
	//	共通設定
	if (mord != NONE_BLEND) {
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	}

	switch (mord)
	{
	case ADD_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case SUB_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case INV_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blenddesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case ALPHA_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		break;
	}
}

void GPipeline::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;							//	定数バッファビュー
	rootParam.Descriptor.ShaderRegister = shaderRegister;	//	定数バッファ番号
	rootParam.Descriptor.RegisterSpace = registerSpace;		//	デフォルト値
	rootParam.ShaderVisibility = shaderVisibility;			//	すべてのシェーダから見る
}

void GPipeline::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescriptorRange, UINT numDescriptorRanges, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;
	rootParam.DescriptorTable.pDescriptorRanges = &pDescriptorRange;
	rootParam.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
	rootParam.ShaderVisibility = shaderVisibility;
}

void GPipeline::SetRootSignature(UINT rootParamNum, int textureNum)
{

#pragma region	ルートパラメータ
	//	ルートパラメータの設定
	std::vector<D3D12_ROOT_PARAMETER> rootParams = {};
	rootParams.resize(rootParamNum + textureNum);
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange{};
	descriptorRange.resize(textureNum);
	for (size_t i = 0; i < textureNum; i++)
	{
		//デスクリプタレンジの設定
		descriptorRange[i].NumDescriptors = 1;
		descriptorRange[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange[i].BaseShaderRegister = (UINT)i;
		descriptorRange[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
	for (size_t i = 0; i < textureNum; i++)
	{
		rootParams[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[i].DescriptorTable.pDescriptorRanges = &descriptorRange[i];
		rootParams[i].DescriptorTable.NumDescriptorRanges = 1;
		rootParams[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		//SetRootParam(rootParams[i], D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, descriptorRange[i], 1);
	}
	for (size_t i = 0; i < rootParamNum; i++)
	{
		SetRootParam(rootParams[i + textureNum], D3D12_ROOT_PARAMETER_TYPE_CBV, (UINT)i, 0);
	}
#pragma endregion

#pragma region sampler
	//	テクスチャーサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
#pragma endregion

#pragma region ルートシグネチャ
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();						//	先頭アドレス
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();					//	ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		rootSigBlob.ReleaseAndGetAddressOf(),
		errorBlob.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));
	result = dx->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion
}

GPipeline::GPipeline(Shader shader, int mord, int constBuffNum)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};

	Init(shader, inputLayout, _countof(inputLayout), constBuffNum);
	SetBlend(mord);
}

void GPipeline::SetBlend(int mord)
{
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blend(blenddesc, mord);
	HRESULT result = dx->GetDev()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&state));
	assert(SUCCEEDED(result));
	for (int i = 0; i < 2 - 1; i++)
	{
		pipelineDesc.BlendState.RenderTarget[i + 1] = pipelineDesc.BlendState.RenderTarget[i];
	}
}

void GPipeline::SetShader(Shader shader)
{
#pragma region VertexShader
	pipelineDesc.VS.pShaderBytecode = shader.VSBlob()->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader.VSBlob()->GetBufferSize();
#pragma endregion
#pragma region HShader
	if (shader.HSBlob() != nullptr) {
		pipelineDesc.HS.pShaderBytecode = shader.HSBlob()->GetBufferPointer();
		pipelineDesc.HS.BytecodeLength = shader.HSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region DShader
	if (shader.DSBlob() != nullptr) {
		pipelineDesc.DS.pShaderBytecode = shader.DSBlob()->GetBufferPointer();
		pipelineDesc.DS.BytecodeLength = shader.DSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region GShader
	if (shader.GSBlob() != nullptr) {
		pipelineDesc.GS.pShaderBytecode = shader.GSBlob()->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = shader.GSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region PixcelShader
	pipelineDesc.PS.pShaderBytecode = shader.PSBlob()->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader.PSBlob()->GetBufferSize();
#pragma endregion
}
