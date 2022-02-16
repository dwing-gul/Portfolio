#ifndef _BAR
#define _BAR

#include "value.fx"

#define RATIO g_float_0
#define DIR g_int_0
#define USE g_int_1
#define BAR_TEX g_texture_0
#define BAR_DECO g_texture_1

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

VERTEX_OUT VS_Bar(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;

	float4 projectionPos = mul(float4(_in.pos, 1.f), g_matrixWVP);

	output.position = projectionPos;
	output.UV = _in.UV;

	return output;
}

float4 PS_Bar(VERTEX_OUT _in) : SV_Target
{
	float4 outColor = float4(1.f, 0.f, 1.f, 1.f);
	float4 decoColor = (float4) 1.f;
	
	if (btex_0)
	{
		outColor = BAR_TEX.Sample(g_sampler_0, _in.UV);
	}
	
	if (btex_1)
	{
		decoColor = BAR_DECO.Sample(g_sampler_0, _in.UV);
	}
	
	if (0 == DIR) // 오른쪽에서 왼쪽
	{
		if (RATIO < _in.UV.x)
		{
			clip(-1);
		}
		
		if(decoColor.a > 0.f)
			outColor *= decoColor;
	}
	else if (1 == DIR) // 왼쪽에서 오른쪽
	{
		if (RATIO < 1.f - _in.UV.x)
		{
			clip(-1);
		}
		
		if (decoColor.a > 0.f)
			outColor *= decoColor;
	}
	else if (2 == DIR) // 위쪽에서 아랫쪽
	{
		if (0 != USE)
		{
			if (0.f < RATIO && RATIO > _in.UV.y)
			{
				outColor.xyz *= 0.8f;
			}
		}
	}
	else if (3 == DIR) // 아랫쪽에서 위쪽
	{
		if (0 != USE)
		{
			if (0.f < RATIO && RATIO > 1.f - _in.UV.y)
			{
				outColor.xyz *= 0.8f;
			}
		}
	}
	else if(4 == DIR) // 바랑 상관없이 아이템 박스를 위한 용도
	{
		if (decoColor.a > 0.9f)
			outColor = decoColor;
	}
	
	return outColor;
}

#endif