#ifndef _FRUSTUM
#define _FRUSTUM

#include "value.fx"

StructuredBuffer<float3> g_Frustum : register(t13);

struct VS_IN
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VS_OUT
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
};

struct GS_OUT
{
	float4 position : SV_Position;
	float2 UV : TEXCOORD;
};

VS_OUT VS_Frustum(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	
	output.pos = _in.pos;
	output.UV = _in.UV;

	return output;
}

[maxvertexcount(30)]
void GS_Frustum(point VS_OUT _in[1], inout LineStream<GS_OUT> _outputStream)
{
	GS_OUT output[8] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
	
	float3 local[8] =
	{
		(float3) 0.f,
		(float3) 0.f,
		(float3) 0.f,
		(float3) 0.f,
		(float3) 0.f,
		(float3) 0.f,
		(float3) 0.f,
		(float3) 0.f
	};
	
	for (int i = 0; i < 8; ++i)
	{
		local[i] = mul(float4(g_Frustum[i], 1.f), g_worldMatrixInverse).xyz;
	}
	
	output[0].position = mul(float4(_in[0].pos + local[0], 1.f), g_matrixWVP);
	output[1].position = mul(float4(_in[0].pos + local[1], 1.f), g_matrixWVP);
	output[2].position = mul(float4(_in[0].pos + local[2], 1.f), g_matrixWVP);
	output[3].position = mul(float4(_in[0].pos + local[3], 1.f), g_matrixWVP);
															   
	output[4].position = mul(float4(_in[0].pos + local[4], 1.f), g_matrixWVP);
	output[5].position = mul(float4(_in[0].pos + local[5], 1.f), g_matrixWVP);
	output[6].position = mul(float4(_in[0].pos + local[6], 1.f), g_matrixWVP);
	output[7].position = mul(float4(_in[0].pos + local[7], 1.f), g_matrixWVP);

	output[0].UV = float2(0.f, 0.f);
	output[1].UV = float2(1.f, 0.f);
	output[2].UV = float2(1.f, 1.f);
	output[3].UV = float2(0.f, 1.f);

	output[4].UV = float2(0.f, 0.f);
	output[5].UV = float2(1.f, 0.f);
	output[6].UV = float2(1.f, 1.f);
	output[7].UV = float2(0.f, 1.f);

	// near
	_outputStream.Append(output[0]);
	_outputStream.Append(output[3]);
	_outputStream.Append(output[2]);
	_outputStream.Append(output[1]);
	_outputStream.Append(output[0]);
	_outputStream.RestartStrip();
	
	// far
	_outputStream.Append(output[4]);
	_outputStream.Append(output[5]);
	_outputStream.Append(output[6]);
	_outputStream.Append(output[7]);
	_outputStream.Append(output[4]);
	_outputStream.RestartStrip();
	
	// up
	_outputStream.Append(output[0]);
	_outputStream.Append(output[1]);
	_outputStream.Append(output[5]);
	_outputStream.Append(output[4]);
	_outputStream.Append(output[0]);
	_outputStream.RestartStrip();
	
	// down
	_outputStream.Append(output[3]);
	_outputStream.Append(output[2]);
	_outputStream.Append(output[6]);
	_outputStream.Append(output[7]);
	_outputStream.Append(output[3]);

	_outputStream.RestartStrip();
	
	// left
	_outputStream.Append(output[0]);
	_outputStream.Append(output[3]);
	_outputStream.Append(output[7]);
	_outputStream.Append(output[4]);
	_outputStream.Append(output[0]);

	_outputStream.RestartStrip();
	
	// right
	_outputStream.Append(output[2]);
	_outputStream.Append(output[1]);
	_outputStream.Append(output[5]);
	_outputStream.Append(output[6]);
	_outputStream.Append(output[2]);

	_outputStream.RestartStrip();
}

float4 PS_Frustum(GS_OUT _in) : SV_Target
{
	float4 outColor = float4(1.f, 0.f, 1.f, 1.f);
	
	return outColor;
}
#endif