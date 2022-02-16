#ifndef _NUMBER
#define _NUMBER

#include "value.fx"

#define NUM_LENGTH g_int_0
#define TOTAL_NUM g_int_1
#define NUM_TEX g_texture_0

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

VERTEX_OUT VS_Number(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;

	float4 projectionPos = mul(float4(_in.pos, 1.f), g_matrixWVP);

	output.position = projectionPos;
	output.UV = _in.UV;

	return output;
}

float4 PS_Number(VERTEX_OUT _in) : SV_Target
{
	float4 outColor = (float4) 0.f;
	
	float2 textureSliceCount = { 10.f, 1.f };
	float2 count = { NUM_LENGTH, 1.f };
	
	float2 cutUV = 1.f / textureSliceCount;
	float2 index;
	float2 UV = modf(_in.UV * count, index);
	
	int numCount = count.x - 1 - index.x;
	int digit = 1;
	while (0 < numCount)
	{
		digit *= 10;
		--numCount;
	}
	
	int output = TOTAL_NUM / digit;
	float2 outputUV = UV / textureSliceCount + (cutUV * output);
	
	outColor = NUM_TEX.Sample(g_sampler_1, outputUV);
	
	if (outColor.a > 0.3f)
	{
		outColor.a = 1.f;
	}
	
	return outColor;
}
#endif