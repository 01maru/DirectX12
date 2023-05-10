#include "ObjHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);


PSOutput main(VSOutput input)
{
	PSOutput output;
	float3 eyedir = normalize(cameraPos - input.worldpos);
	const float shininess = 20.0f;
	float4 shadercolor;
	shadercolor.a = m_alpha;
	float4 rimLight;
	//	•ûŒüƒ‰ƒCƒg
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			float3 dolightnormal = saturate(dot(dirLights[i].lightv, input.normal));
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dolightnormal * input.normal);


			float3 ambient = m_ambient;
			float3 diffuse = dolightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			if (isToon) {
				float toonColor = 0.1;
				diffuse = smoothstep(toonColor, toonColor + 0.05, diffuse);
				specular = smoothstep(toonColor, toonColor + 0.05, specular);
			}
			float3 ads = ambient + diffuse + specular;

			if (isRim) {
				float rimDotValue = dot(eyedir, input.normal);
				float rim = (1 - step(0.1, rimDotValue));

				shadercolor.rgb += (1 - rim) * ads * dirLights[i].lightcolor;
				rimLight += rim * float4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else {
				shadercolor.rgb += ads * dirLights[i].lightcolor;
			}
		}
	}

	float4 texcolor = float4(tex.Sample(smp, input.uv)) * float4(color, 1.0f);

	output.target0 = shadercolor * texcolor + rimLight;
	return output;
}