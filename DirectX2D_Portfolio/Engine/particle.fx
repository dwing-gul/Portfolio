#ifndef _PARTICLE
#define _PARTICLE
#include "value.fx"

#define START_SCALE  g_vector4_0
#define END_SCALE	 g_vector4_1
#define START_COLOR  g_vector4_2
#define END_COLOR	 g_vector4_3
#define PARTICLE_TEX g_texture_0
#define ANISOTROPIC g_sampler_0
#define MINIMUM_MIN_MAG_MIP_POINT g_sampler_1

StructuredBuffer<tParticle> g_particle : register(t12);

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	uint instID : SV_InstanceID;
};

struct VS_OUT
{
	float3 viewPos : POSITION;
	float2 uv : TEXCOORD;
	float instID : FOG;
};

struct GS_OUT
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD;
	uint instID : SV_InstanceID;
};

VS_OUT VS_Particle(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	float3 worldPos = g_particle[_in.instID].worldPos;

	output.viewPos = mul(float4(worldPos, 1.f), g_viewMatrix).xyz;
	output.uv = _in.uv;
	output.instID = _in.instID;

	return output;
}

[maxvertexcount(6)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _outputStream)
{
	GS_OUT output[4] = { (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f };

	if (0 == g_particle[_in[0].instID].alive)
		return;

	float lifeRatio = g_particle[(int)_in[0].instID].curTime / g_particle[(int)_in[0].instID].maxTime;
	float3 curScale = ((END_SCALE.xyz - START_SCALE.xyz) * lifeRatio + START_SCALE.xyz) / 2.f;

	output[0].position = mul(float4(_in[0].viewPos + float3(-curScale.x, curScale.y, 0.f), 1.f), g_projectionMatrix);
	output[1].position = mul(float4(_in[0].viewPos + float3(curScale.x, curScale.y, 0.f), 1.f), g_projectionMatrix);
	output[2].position = mul(float4(_in[0].viewPos + float3(curScale.x, -curScale.y, 0.f), 1.f), g_projectionMatrix);
	output[3].position = mul(float4(_in[0].viewPos + float3(-curScale.x, -curScale.y, 0.f), 1.f), g_projectionMatrix);

	output[0].uv = float2(0.f, 0.f);
	output[1].uv = float2(1.f, 0.f);
	output[2].uv = float2(1.f, 1.f);
	output[3].uv = float2(0.f, 1.f);

	output[0].instID = (int)_in[0].instID;
	output[1].instID = (int)_in[0].instID;
	output[2].instID = (int)_in[0].instID;
	output[3].instID = (int)_in[0].instID;

	_outputStream.Append(output[0]);
	_outputStream.Append(output[2]);
	_outputStream.Append(output[3]);
	_outputStream.RestartStrip();

	_outputStream.Append(output[2]);
	_outputStream.Append(output[0]);
	_outputStream.Append(output[1]);
	_outputStream.RestartStrip();
}

float4 PS_Particle(GS_OUT _in) : SV_Target
{
	float4 outColor = (float4)0.f;

	float lifeRatio = g_particle[_in.instID].curTime / g_particle[_in.instID].maxTime;
	float3 curColor = (END_COLOR.xyz - START_COLOR.xyz) * lifeRatio + START_COLOR.xyz;

	outColor = PARTICLE_TEX.Sample(ANISOTROPIC, _in.uv);
	outColor.rgb *= curColor;

	return outColor;
}
#endif
