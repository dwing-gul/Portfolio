#ifndef _COPYTEXTURE
#define _COPYTEXTURE

#include "value.fx"
#include "func.fx"

RWTexture2D<float4> g_destTex : register(u0);
#define SRC_TEX g_texture_0
#define IsSampling g_int_0
#define SrcResolution g_vec2_0
#define DestResolution g_vec2_1

[numthreads(32, 32, 1)]
void CS_CopyTexture(int3 _threadID : SV_DispatchThreadID)
{
	if (DestResolution.x <= _threadID.x || DestResolution.y <= _threadID.y)
		return;

	float2 uv = float2(_threadID.x / (float) DestResolution.x, _threadID.y / (float) DestResolution.y);
	int2 pixelPos = int2(uv.x * SrcResolution.x, uv.y * SrcResolution.y);
	
	float4 color = (float4) 0.f;
	
	if (IsSampling)
	{
		color = Sample_CS(pixelPos, SRC_TEX, SrcResolution);
	}
	else
	{
		color = SRC_TEX[pixelPos];
	}
	
	color.a = 1.f;
	
	g_destTex[_threadID.xy] = color;
}

#endif