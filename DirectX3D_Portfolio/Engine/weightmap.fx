#ifndef _WEIGHTMAP
#define _WEIGHTMAP

#include "value.fx"

struct tWeight_4
{
	float arrWeight[4];
};

RWStructuredBuffer<tWeight_4> WEIGHT_MAP : register(u0);
StructuredBuffer<tRaycastOut> LOCATION : register(t13);
StructuredBuffer<int> INDEX : register(t14);

#define BRUSH_TEX g_texture_arr_0

#define WIDTH g_int_0
#define HEIGHT g_int_1
#define BRUSH_INDEX g_int_2
#define WEIGHT_INDEX g_int_3

#define SCALE g_vec2_0

[numthreads(32, 32, 1)]
void CS_WeightMap(int3 _threadID : SV_DispatchThreadID)
{
	if (WIDTH <= _threadID.x || HEIGHT <= _threadID.y || !LOCATION[0].success)
		return;
	
	float2 brushUV = LOCATION[0].UV;
	
	int2 centerPos = float2(WIDTH, HEIGHT) * brushUV;
	int2 scale = float2(WIDTH, HEIGHT) * SCALE;
	
	if (_threadID.x < centerPos.x - (scale.x / 2) || centerPos.x + (scale.x / 2) < _threadID.x ||
		_threadID.y < centerPos.y - (scale.y / 2) || centerPos.y + (scale.y / 2) < _threadID.y)
	{
		return;
	}
	
	int2 LTPos = centerPos - (scale / 2);
	float2 uv = float2(_threadID.xy - LTPos) / float2(scale);
	
	float4 brushColor = BRUSH_TEX.SampleLevel(g_sampler_0, float3(uv, BRUSH_INDEX), 0);
	
	uint index = (_threadID.y * WIDTH) + _threadID.x;

	float arrWeight[4] = WEIGHT_MAP[index].arrWeight;
	float add = g_deltaTime * 0.5f * brushColor.a;

	arrWeight[WEIGHT_INDEX] = saturate(arrWeight[WEIGHT_INDEX] + add);

	float total = 0.f;
	
	for (int i = 0; i < 4; ++i)
	{
		total += arrWeight[i];
	}
	
	if (1.f < total)
	{
	
		float over = total - 1.f;
	
		total = total - arrWeight[WEIGHT_INDEX];
	
		for (int j = 0; j < 4; ++j)
		{
			if (j == WEIGHT_INDEX)
				continue;
		
			arrWeight[j] = saturate(arrWeight[j] - over * (arrWeight[j] / total));
		}
	}
	
	WEIGHT_MAP[index].arrWeight = arrWeight;
}


#endif