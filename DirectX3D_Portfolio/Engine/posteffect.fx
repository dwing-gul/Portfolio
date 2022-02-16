#ifndef _POSTEFFECT
#define _POSTEFFECT

#include "value.fx"

#define CLONE_TEX g_texture_0

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

VERTEX_OUT VS_Distortion(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	output.position = float4(_in.pos, 1.f);
	output.UV = _in.UV;
	
	return output;
}


float4 PS_Distortion(VERTEX_OUT _in) :SV_Target
{
	float2 screenUV = _in.position.xy / g_resolution;
	
	screenUV.y += sin(screenUV.x * 20.f + (g_accTime * 12.56)) * 0.01f;
	
	float4 color = CLONE_TEX.Sample(g_sampler_0, screenUV);
	return color;
}

#endif