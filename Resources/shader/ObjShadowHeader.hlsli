cbuffer ConstBufferDataTransform:register(b1)
{
	matrix matview;
	matrix matworld;
	float3 cameraPos;
};

//cbuffer skinning:register(b2)
//{
//	matrix matSkinning[MAX_BONES];
//}

struct VSOutput
{
	//	�Z�}���e�B�b�N(SV_��System Value)
	float4 svpos : SV_POSITION;	//	���W���
};