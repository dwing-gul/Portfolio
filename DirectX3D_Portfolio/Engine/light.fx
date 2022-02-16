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
	
	// �׸��� ����    
    // ���� ������ �׸��� ó���� ���� �ʴ´�.
	if (dot(color.diffuse, color.diffuse) != 0.f)
	{
		float4 worldPos = mul(float4(viewPos.xyz, 1.f), g_viewMatrixInverse); // ����ī�޶� view ������� ���ؼ� ������ǥ�� �˾Ƴ���.
		float4 shadowProjection = mul(worldPos, LightCamViewProjectionMatrix); // ���� �������� ������Ų ��ǥ ���ϱ�
		float depth = shadowProjection.z / shadowProjection.w; // w �� ������ ���� ������ǥ z ���� ���Ѵ�.(�ùٸ��� ���ϱ� ���ؼ�)
    
        // ���� ������ǥ�� UV ��ǥ�� �����ؼ� ShadowMap �� ��ϵ� ���̰��� �����´�.
		float2 shadowUV = float2((shadowProjection.x / shadowProjection.w) * 0.5f + 0.5f, (shadowProjection.y / shadowProjection.w) * -0.5f + 0.5f);
    
		if (0.01f < shadowUV.x && shadowUV.x < 0.99f && 0.01f < shadowUV.y && shadowUV.y < 0.99f)
		{
			float shadowDepth = DYNAMIC_SHADOW_DEPTH.Sample(g_sampler_0, shadowUV).r;
      
            // �׸����� ��� ���� ��ȭ��Ų��.
            // ������Ų ���̶�, ��ϵ� ���� �������� �׸��ڰ� ����� �ȵǴµ�, �Ǽ������� �߻��� �� �ֱ� ������ �������� �ش�.
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