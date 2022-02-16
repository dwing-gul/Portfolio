#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"

#define NORMAL_TARGET g_texture_0
#define POSITION_TARGET g_texture_1
#define LIGHT_INDEX g_int_0

#define DYNAMIC_SHADOW_DEPTH    g_texture_3
#define LightCamViewProjectionMatrix     g_matrix_0

struct VERTEX_IN
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VERTEX_OUT
{
	float4 position : SV_Position;
	float2 UV : TEXCOORD;
};

struct PS_OUT
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1;
};

VERTEX_OUT VS_DirLight(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	output.position = float4(_in.pos.xy * 2.f, 0.f, 1.f);
	output.UV = _in.UV;

	return output;
}

PS_OUT PS_DirLight(VERTEX_OUT _in)
{
	PS_OUT output = (PS_OUT) 0.f;

	float3 viewPos = POSITION_TARGET.Sample(g_sampler_0, _in.UV).xyz;
	
	if (viewPos.z == 0.f)
		clip(-1);
	
	float3 viewNormal = NORMAL_TARGET.Sample(g_sampler_0, _in.UV).xyz;
	
	tLightColor color = (tLightColor) 0.f;
	CalculateLighting(viewNormal, viewPos, color, LIGHT_INDEX);
	
	// 그림자 판정    
    // 빛이 없으면 그림자 처리를 하지 않는다.
	if (dot(color.diffuse, color.diffuse) != 0.f)
	{
		float4 worldPos = mul(float4(viewPos.xyz, 1.f), g_viewMatrixInverse); // 메인카메라 view 역행렬을 곱해서 월드좌표를 알아낸다.
		float4 shadowProjection = mul(worldPos, LightCamViewProjectionMatrix); // 광원 시점으로 투영시킨 좌표 구하기
		float depth = shadowProjection.z / shadowProjection.w; // w 로 나눠서 실제 투영좌표 z 값을 구한다.(올바르게 비교하기 위해서)
    
        // 계산된 투영좌표를 UV 좌표로 변경해서 ShadowMap 에 기록된 깊이값을 꺼내온다.
		float2 shadowUV = float2((shadowProjection.x / shadowProjection.w) * 0.5f + 0.5f, (shadowProjection.y / shadowProjection.w) * -0.5f + 0.5f);
    
		if (0.01f < shadowUV.x && shadowUV.x < 0.99f && 0.01f < shadowUV.y && shadowUV.y < 0.99f)
		{
			float shadowDepth = DYNAMIC_SHADOW_DEPTH.Sample(g_sampler_0, shadowUV).r;
      
            // 그림자인 경우 빛을 약화시킨다.
            // 투영시킨 값이랑, 기록된 값이 같은경우는 그림자가 생기면 안되는데, 실수오차가 발생할 수 있기 때문에 보정값을 준다.
			if (shadowDepth != 0.f && (depth > shadowDepth + 0.00001f))
			{
				color.diffuse *= 0.3f;
				color.specular = (float4) 0.f;
				color.ambient *= 0.5f;
			}
		}
	}
	
	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;
	
	return output;
}

VERTEX_OUT VS_PointLight(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);

	return output;
}

PS_OUT PS_PointLight(VERTEX_OUT _in)
{
	PS_OUT output = (PS_OUT) 0.f;
	
	float2 screenUV = _in.position.xy / g_resolution;

	float3 viewPos = POSITION_TARGET.Sample(g_sampler_0, screenUV).xyz;
	
	if (viewPos.z == 0.f)
	{
		output.diffuse = float4(0.f, 1.f, 0.f, 1.f);
		return output;
	}
	
	float4 worldPos = mul(float4(viewPos, 1.f), g_viewMatrixInverse);
	float4 localPos = mul(worldPos, g_worldMatrixInverse);
	
	if (1.f < length(localPos.xyz))
	{
		output.diffuse = float4(0.f, 0.f, 1.f, 1.f);
		return output;
	}
	
	float3 viewNormal = NORMAL_TARGET.Sample(g_sampler_0, screenUV).xyz;
	
	tLightColor color = (tLightColor) 0.f;
	CalculateLighting(viewNormal, viewPos, color, LIGHT_INDEX);
	
	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;
	
	return output;
}

#define COLOR_TARGET g_texture_0
#define DIFFUSE_TARGET g_texture_1
#define SPECULAR_TARGET g_texture_2
#define BRUSH g_int_0

VERTEX_OUT VS_LightMerge(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	output.position = float4(_in.pos.xy * 2.f, 0.f, 1.f);
	output.UV = _in.UV;

	return output;
}

float4 PS_LightMerge(VERTEX_OUT _in) : SV_Target
{
	float4 outColor = (float4) 0.f;
	
	float4 colorTarget = COLOR_TARGET.SampleLevel(g_sampler_1, _in.UV, 0.f);
	float4 diffuse = DIFFUSE_TARGET.SampleLevel(g_sampler_0, _in.UV, 0.f);
	float4 specular = SPECULAR_TARGET.SampleLevel(g_sampler_0, _in.UV, 0.f);
	
	float4 color = decode(colorTarget.r);
	float4 spcularCoefficient = decode(colorTarget.g);
	
	if (BRUSH)
		color.rgb += float3(0.7f, 0.3f, 0.5f) * color.a;
	
	outColor = color * diffuse + specular * spcularCoefficient;
	
	return outColor;
}

#endif