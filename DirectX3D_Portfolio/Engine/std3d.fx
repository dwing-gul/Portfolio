#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"

struct VERTEX_IN
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
	
	float3 tangent : TANGENT;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
};

struct VERTEX_OUT
{
	float4 position : SV_Position;
	float2 UV : TEXCOORD;
	float3 viewPos : POSITION;
	
	float3 viewTangent : TANGENT;
	float3 viewNormal : NORMAL;
	float3 viewBinormal : BINORMAL;
};

VERTEX_OUT VS_Std3D(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.UV = _in.UV;
	
	output.viewPos = mul(float4(_in.pos, 1.f), g_matrixWV).xyz;
	
	output.viewTangent = normalize(mul(float4(_in.tangent, 0.f), g_matrixWV).xyz);
	output.viewNormal = normalize(mul(float4(_in.normal, 0.f), g_matrixWV).xyz);
	output.viewBinormal = normalize(mul(float4(_in.binormal, 0.f), g_matrixWV).xyz);

	return output;
}

float4 PS_Std3D(VERTEX_OUT _in) : SV_Target
{
	float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
	float3 viewNormal = _in.viewNormal;
	
	if (btex_0)
	{
		outColor = g_texture_0.Sample(g_sampler_0, _in.UV);
	}
	
	if (btex_1)
	{
		float3 tangentSpaceNormal = g_texture_1.Sample(g_sampler_0, _in.UV).xyz;
		tangentSpaceNormal = tangentSpaceNormal * 2.f - 1.f;

		float3x3 matrixTBN = { _in.viewTangent, _in.viewBinormal, _in.viewNormal };
		viewNormal = mul(tangentSpaceNormal, matrixTBN);
	}
	
	tLightColor _lightColor = (tLightColor) 0.f;
	
	for (int i = 0; i < g_light3DCount; ++i)
	{
		CalculateLighting(viewNormal, _in.viewPos, _lightColor, i);
	}
	
	outColor.xyz = outColor.xyz * _lightColor.diffuse.xyz + _lightColor.specular.xyz + outColor.xyz * _lightColor.ambient.xyz;
	
	if (btex_cube_0)
	{
		float3 eye = normalize(_in.viewPos);
		float3 eyeReflect = normalize(reflect(eye, viewNormal));
		float4 reflectColor = g_texture_cube_0.Sample(g_sampler_0, eyeReflect);
		
		outColor.xyz *= reflectColor.xyz;
	}
	
	return outColor;
}

VERTEX_OUT VS_SkyBox(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	float4 viewPos = mul(float4(_in.pos, 0.f), g_viewMatrix);
	float4 projectionPos = mul(viewPos, g_projectionMatrix);
	projectionPos.z = projectionPos.w;
	
	output.position = projectionPos;
	output.UV = _in.UV;
	output.viewPos = normalize(_in.pos);

	return output;
}

float4 PS_SkyBox(VERTEX_OUT _in) : SV_Target
{
	float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
	float3 viewNormal = _in.viewNormal;
	
	if (btex_0)
	{
		outColor = g_texture_0.Sample(g_sampler_0, _in.UV);
	}
	
	if (btex_cube_0)
	{
		outColor = g_texture_cube_0.Sample(g_sampler_0, _in.viewPos);
	}
	
	return outColor;
}

VERTEX_OUT VS_Collider3D(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;

	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);

	return output;
}

float4 PS_Collider3D(VERTEX_OUT _in) : SV_Target
{
	float4 pos = _in.position;
	
	if (g_int_0)
	{
		return float4(0.9f, 0.2f, 0.2f, 1.f);
	}

	return float4(0.2f, 0.9f, 0.2f, 1.f);
}

#endif