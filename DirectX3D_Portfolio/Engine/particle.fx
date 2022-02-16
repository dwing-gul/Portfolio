#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"

#define g_startScale g_vec4_0
#define g_endScale g_vec4_1

#define g_startColor g_vec4_2
#define g_endColor g_vec4_3

#define g_particleTex g_texture_0

StructuredBuffer<tParticle> g_particle : register(t12);

struct VS_IN
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;

	uint instID :SV_InstanceID;
};

struct VS_OUT
{
	float3 viewPos : POSITION;
	float2 UV : TEXCOORD;
	float instID :FOG;
};

struct GS_OUT
{
	float4 position : SV_Position;
	float2 UV : TEXCOORD;
	uint instID : SV_InstanceID;
};

VS_OUT VS_Particle(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	float3 worldPos = g_particle[_in.instID].worldPos;

	output.viewPos = mul(float4(worldPos, 1.f), g_viewMatrix).xyz;
	output.UV = _in.UV;
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
	float3 curScale = (((g_endScale - g_startScale) * lifeRatio + g_startScale) / 2.f).xyz;

	output[0].position = mul(float4(_in[0].viewPos + float3(-curScale.x, curScale.y, 0.f), 1.f), g_projectionMatrix);
	output[1].position = mul(float4(_in[0].viewPos + float3(curScale.x, curScale.y, 0.f), 1.f), g_projectionMatrix);
	output[2].position = mul(float4(_in[0].viewPos + float3(curScale.x, -curScale.y, 0.f), 1.f), g_projectionMatrix);
	output[3].position = mul(float4(_in[0].viewPos + float3(-curScale.x, -curScale.y, 0.f), 1.f), g_projectionMatrix);

	output[0].UV = float2(0.f, 0.f);
	output[1].UV = float2(1.f, 0.f);
	output[2].UV = float2(1.f, 1.f);
	output[3].UV = float2(0.f, 1.f);

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

float4 PS_Particle(GS_OUT _in) :SV_Target
{
	float4 outColor = (float4)0.f;

	float lifeRatio = g_particle[_in.instID].curTime / g_particle[_in.instID].maxTime;
	float3 curColor = ((g_endColor - g_startColor) * lifeRatio + g_startColor).xyz;

	outColor = g_particleTex.Sample(g_sampler_0, _in.UV);
	outColor.rgb *= curColor;

	return outColor;
}

#endif