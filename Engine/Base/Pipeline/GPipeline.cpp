#include "GPipeline.h"
#include "Shader.h"
#include "DirectX.h"
#include <cassert>
#include <vector>

void GPipeline::SetShader(Shader& shader)
{
#pragma region VertexShader
	pipelineDesc_.VS.pShaderBytecode = shader.VSBlob()->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = shader.VSBlob()->GetBufferSize();
#pragma endregion
#pragma region HShader
	if (shader.HSBlob() != nullptr) {
		pipelineDesc_.HS.pShaderBytecode = shader.HSBlob()->GetBufferPointer();
		pipelineDesc_.HS.BytecodeLength = shader.HSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region DShader
	if (shader.DSBlob() != nullptr) {
		pipelineDesc_.DS.pShaderBytecode = shader.DSBlob()->GetBufferPointer();
		pipelineDesc_.DS.BytecodeLength = shader.DSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region GShader
	if (shader.GSBlob() != nullptr) {
		pipelineDesc_.GS.pShaderBytecode = shader.GSBlob()->GetBufferPointer();
		pipelineDesc_.GS.BytecodeLength = shader.GSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region PixcelShader
	pipelineDesc_.PS.pShaderBytecode = shader.PSBlob()->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = shader.PSBlob()->GetBufferSize();
#pragma endregion
}

void GPipeline::SetBlendDesc(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, int mord)
{
	//	ブレンドなしだったら
	if (mord == NONE_BLEND) return;

	//	共通設定
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

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
	result = MyDirectX::GetInstance()->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion
}

void GPipeline::Init(Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, int constBuffNum, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord, D3D12_CULL_MODE cullmord, D3D12_DEPTH_WRITE_MASK depth_write_mask, bool isDeep, DXGI_FORMAT format, int textureNum)
{
	HRESULT result;
	// シェーダーの設定
	SetShader(shader);

	// サンプルマスクの設定
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

#pragma region Rasterizer
	// 設定
	pipelineDesc_.RasterizerState.CullMode = cullmord; // 背面カリング
	pipelineDesc_.RasterizerState.FillMode = fillmord; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
#pragma endregion

#pragma region  Blending
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendDesc(blenddesc);
#pragma endregion

	// 頂点レイアウトの設定
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc_.InputLayout.NumElements = inputLayoutSize;

	// 図形の形状設定
	pipelineDesc_.PrimitiveTopologyType = topologyType;

	// その他の設定
	pipelineDesc_.NumRenderTargets = 2;		// 描画対象
	for (int i = 0; i < 2; i++)
	{
		pipelineDesc_.RTVFormats[i] = format; // 0~255指定のRGBA
	}
	pipelineDesc_.SampleDesc.Count = 1;							  // 1ピクセルにつき1回サンプリング

	//	デプスステンシルステート設定
	if (isDeep) {
		pipelineDesc_.DepthStencilState.DepthEnable = true;								//	深度テストを行う
		pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//	小さければ合格
	}
	else {
		pipelineDesc_.DepthStencilState.DepthEnable = false;
		pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	}
	pipelineDesc_.DepthStencilState.DepthWriteMask = depth_write_mask;		//	書き込み許可するかどうか
	pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//	深度フォーマット

	SetRootSignature(constBuffNum, textureNum);

	// パイプラインにルートシグネチャをセット
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// パイプランステートの生成
	result = MyDirectX::GetInstance()->GetDev()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&state_));
	assert(SUCCEEDED(result));
}

void GPipeline::Update(D3D_PRIMITIVE_TOPOLOGY primitive)
{
	MyDirectX* dx = MyDirectX::GetInstance();
	// パイプラインステートとルートシグネチャの設定コマンド
	dx->GetCmdList()->SetPipelineState(state_.Get());
	dx->GetCmdList()->IASetPrimitiveTopology(primitive);
}

void GPipeline::SetGraphicsRootSignature()
{
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootSignature(rootSignature_.Get());
}

void GPipeline::SetBlendMord(int mord)
{
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	SetBlendDesc(blenddesc, mord);
	HRESULT result = MyDirectX::GetInstance()->GetDev()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&state_));
	assert(SUCCEEDED(result));
	for (int i = 0; i < 2 - 1; i++)
	{
		pipelineDesc_.BlendState.RenderTarget[i + 1] = pipelineDesc_.BlendState.RenderTarget[i];
	}
}
