#ifndef _HEIGHTMAP
#define _HEIGHTMAP

#include "value.fx"
#include "struct.fx"
#include "func.fx"

RWTexture2D<float> HEIGHT_MAP : register(u0);
StructuredBuffer<tRaycastOut> LOCATION : register(t13);
StructuredBuffer<int> INDEX : register(t14);

#define BRUSH_TEX g_texture_arr_0

#define WIDTH g_int_0
#define HEIGHT g_int_1
#define BRUSH_INDEX g_int_2
#define MODE g_int_3

#define SCALE g_vec2_0

[numthreads(32, 32, 1)]
void CS_HeightMap(int3 _threadID : SV_DispatchThreadID)
{
	if (WIDTH <= _threadID.x || HEIGHT <= _threadID.y || !LOCATION[0].success || INDEX[0] == 0)
		return;
	
	float2 brushUV = LOCATION[0].UV;
	
	if (INDEX[0] > 1)
	{
		for (int i = 0; i < INDEX[0] - 1; ++i)
		{
			if (LOCATION[i].success != 1 || LOCATION[i + 1].success != 1)
				continue;
			
			if (LOCATION[i].dist < LOCATION[i + 1].dist)
			{
				brushUV = LOCATION[i].UV;
			}
			else
			{
				brushUV = LOCATION[i + 1].UV;
			}
		}	
	}
	
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
	
	if (0 == MODE)
		HEIGHT_MAP[_threadID.xy] += g_deltaTime * brushColor.a;
	else if (1 == MODE)
		HEIGHT_MAP[_threadID.xy] -= g_deltaTime * brushColor.a;
	else if (2 == MODE)
	{
		float averageHeight = Sample_Flatten(centerPos, HEIGHT_MAP, scale);
		
		if (HEIGHT_MAP[_threadID.xy] < averageHeight)
		{
			HEIGHT_MAP[_threadID.xy] += g_deltaTime * brushColor.a;
		}
		else if (averageHeight < HEIGHT_MAP[_threadID.xy])
		{
			HEIGHT_MAP[_threadID.xy] -= g_deltaTime * brushColor.a;
		}
	}
}

#endif