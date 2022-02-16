#ifndef _POSTEFFECT
#define _POSTEFFECT
#include "value.fx"

#define CLONE_TEX g_texture_0
#define ANISOTROPIC g_sampler_0
#define MINIMUM_MIN_MAG_MIP_POINT g_sampler_1

struct VERTEX_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VERTEX_OUT
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD;
};

VERTEX_OUT VS_Distortion(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	output.position = float4(_in.pos, 1.f);
	output.uv = _in.uv;

	return output;
}

float4 PS_Distortion(VERTEX_OUT _in) : SV_Target
{
	float2 screenUV = _in.position.xy / g_resolution;
	screenUV += sin(screenUV.x * 20.f + (g_accTime * 12.56f)) * 0.01f;
	
	float4 color = CLONE_TEX.Sample(ANISOTROPIC, screenUV);
	return color;
}
#endif