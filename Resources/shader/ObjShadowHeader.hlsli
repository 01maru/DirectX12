cbuffer ConstBufferDataTransform:register(b1)
{
	matrix matview;
	matrix matworld;
	float3 cameraPos;
};

cbuffer CBuffLight:register(b2)
{
	matrix mLVP;
	float3 lightPos;
}

//cbuffer skinning:register(b2)
//{
//	matrix matSkinning[MAX_BONES];
//}

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 svpos : SV_POSITION;	//	座標情報
	float2 depth : TEXCOORD1;   // ライト空間での座標
};