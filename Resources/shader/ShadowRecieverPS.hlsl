#include "ShadowRecieverHeader.hlsli"

Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_shadowMap : register(t1);  // �V���h�E�}�b�v
sampler g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = g_albedo.Sample(g_sampler, input.uv);

    // ���C�g�r���[�X�N���[����Ԃ���UV��Ԃɍ��W�ϊ�
    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    // step-4 ���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z����
    float zInLVP = input.posInLVP.z;

    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).xy;

        // �܂����̃s�N�Z�����Օ�����Ă��邩���ׂ�B����͒ʏ�̃f�v�X�V���h�E�Ɠ���
        if (zInLVP > shadowValue.r && zInLVP <= 1.0f)
        {
            // �Օ�����Ă���Ȃ�A�`�F�r�V�F�t�̕s�����𗘗p���Č���������m�������߂�
            float depth_sq = shadowValue.x * shadowValue.x;

            // ���̃O���[�v�̕��U������߂�
            // ���U���傫���قǁAvariance�̐��l�͑傫���Ȃ�
            float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);

            // ���̃s�N�Z���̃��C�g���猩���[�x�l�ƃV���h�E�}�b�v�̕��ς̐[�x�l�̍������߂�
            float md = zInLVP - shadowValue.x;

            // �����͂��m�������߂�
            float lit_factor = variance / (variance + md * md);

            // �V���h�E�J���[�����߂�
            float3 shadowColor = texcolor.xyz * 0.5f;

            // ����������m�����g���Ēʏ�J���[�ƃV���h�E�J���[����`�⊮
            texcolor.xyz = lerp(shadowColor, texcolor.xyz, lit_factor);
        }

        //// step-3 �V���h�E�}�b�v�ɕ`�����܂�Ă���Z�l�Ɣ�r����
        //// �v�Z����UV���W���g���āA�V���h�E�}�b�v����[�x�l���T���v�����O
        //float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
        //if (zInLVP > zInShadowMap)
        //{
        //    // �Օ�����Ă���
        //    texcolor.xyz *= 0.5f;
        //}
    }
    return texcolor;
}