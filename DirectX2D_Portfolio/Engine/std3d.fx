#ifndef _STD3D
#define _STD3D

#include "value.fx"

static float3 g_LightDir = float3(1.f, -1.f, 1.f);
static float3 g_Ambient = float3(0.2f, 0.2f, 0.2f);

struct VTX_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VTX_OUT
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float lightPower : FOG;
};

VTX_OUT VS_Std3D(VTX_IN _in)
{
	VTX_OUT output = (VTX_OUT) 0.f;
	
	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.uv = _in.uv;
	
	float3 lightDir = normalize(g_LightDir);
	float3 worldDir = normalize(mul(float4(_in.normal, 0.f), g_worldMatrix));
	
	output.lightPower = saturate(dot(lightDir, worldDir));

	return output;
}

float PS_Std3D(VTX_OUT _in) : SV_Target
{
	float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
	
	outColor.xyz = outColor.xyz * _in.lightPower + outColor.xyz * g_Ambient;
	
	return outColor;
}

#endif