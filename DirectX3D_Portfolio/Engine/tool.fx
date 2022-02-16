#ifndef _TOOL
#define _TOOL

#include "value.fx"

struct VERTEX_GRID_IN
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VERTEX_GRID_OUT
{
	float4 position : SV_Position;
	float3 worldPos : POSITION;
	float2 UV : TEXCOORD;
	float3 viewPos : POSITION1;
};

VERTEX_GRID_OUT VS_Grid(VERTEX_GRID_IN _in)
{
	VERTEX_GRID_OUT output = (VERTEX_GRID_OUT) 0.f;
	
	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.worldPos = mul(float4(_in.pos, 1.f), g_worldMatrix).xyz;
	output.viewPos = mul(float4(_in.pos, 1.f), g_matrixWV).xyz;
	output.UV = _in.UV;
	
	return output;
}

float4 PS_Grid(VERTEX_GRID_OUT _in) : SV_Target
{
	float2 fullUV = float2(_in.position.x / g_resolution.x, _in.position.y / g_resolution.y);
	float3 targetViewPos = g_texture_0.Sample(g_sampler_0, fullUV).xyz;
	
	float len = length(targetViewPos);
	float max = len * 0.01f;
	
	float depthRatio = 1.f;
	if (targetViewPos.z != 0.f)
	{
		depthRatio = 1.f - saturate((length(_in.viewPos) - length(targetViewPos)) / max);
	}
	
	float dist = abs(distance(_in.worldPos, g_vec4_0.xyz));
	float thickness = g_float_1 * (dist / abs(g_vec4_0.y)) * 1.2f;
	
	int step = 1;
	for (int i = 0; i < g_int_0 + 1; ++i)
	{
		step *= 10;
	}
	
	if (dist > 200.f && dist > abs(g_vec4_0.y) * 5.f)
	{
		clip(-1);
	}
	
	float x = abs(_in.worldPos.x - (float) ((int) _in.worldPos.x / step) * step);
	float z = abs(_in.worldPos.z - (float) ((int) _in.worldPos.z / step) * step);
	
	if (thickness / 2.f < x)
		x = (float) step - x;
	if (thickness / 2.f < z)
		z = (float) step - z;

	float xRatio = 0.f;
	float zRatio = 0.f;
	float ratio = 0.f;
	
	if (x < thickness / 2.f || z < thickness / 2.f)
	{
		xRatio = ((thickness / 2.f) - abs(x)) / (thickness / 2.f);
		zRatio = ((thickness / 2.f) - abs(z)) / (thickness / 2.f);
		
		ratio = 0.f;
		if (xRatio > zRatio)
			ratio = xRatio;
		else
			ratio = zRatio;
		
		return float4(g_vec4_1.rgb, ratio * depthRatio);
	}
	
	step = 1;
	for (i = 0; i < g_int_0; ++i)
	{
		step *= 10;
	}
	
	x = abs(_in.worldPos.x - (float) ((int) _in.worldPos.x / step) * step);
	z = abs(_in.worldPos.z - (float) ((int) _in.worldPos.z / step) * step);
	
	if (thickness / 2.f < x)
		x = (float) step - x;
	if (thickness / 2.f < z)
		z = (float) step - z;

	xRatio = 0.f;
	zRatio = 0.f;
	ratio = 0.f;
	
	if (x < thickness / 2.f || z < thickness / 2.f)
	{
		xRatio = ((thickness / 2.f) - abs(x)) / (thickness / 2.f);
		zRatio = ((thickness / 2.f) - abs(z)) / (thickness / 2.f);
		
		ratio = 0.f;
		if (xRatio > zRatio)
			ratio = xRatio;
		else
			ratio = zRatio;
		
		return float4(g_vec4_1.rgb, ratio * g_float_0 * depthRatio);
	}
	
	clip(-1);
	return float4(0.f, 0.f, 0.f, 0.f);
}
#endif