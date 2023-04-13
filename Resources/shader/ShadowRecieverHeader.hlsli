// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    matrix mViewProj;
    matrix mWorld;
};

// ���C�g�r���[�v���W�F�N�V�����s��̒萔�o�b�t�@�[���`
cbuffer ShadowCb : register(b1)
{
    matrix mLVP;
};

struct VSOutput
{
    float4 pos : SV_POSITION;       // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL;         // �@��
    float2 uv : TEXCOORD0;          // UV���W

    // step-4 ���C�g�r���[�X�N���[����Ԃł̍��W��ǉ�
    float4 posInLVP : TEXCOORD1;    // ���C�g�r���[�X�N���[����Ԃł̃s�N�Z���̍��W
};