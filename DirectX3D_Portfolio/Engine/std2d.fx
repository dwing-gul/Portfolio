#ifndef _STD2D
#define _STD2D

#include "value.fx"

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

// Vertex Shader
VERTEX_OUT VS_Std2D(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT)0.f;

	float4 projectionPos = mul(float4(_in.pos, 1.f), g_matrixWVP);

	output.position = projectionPos;
	output.UV = _in.UV;

	return output;
}

// Pixel Shader
float4 PS_Std2D(VERTEX_OUT _in) : SV_Target
{
	float4 outColor = float4(1.f, 0.f, 1.f, 1.f);

	if (bAnim2D)
	{
		float2 finalLeftTop = leftTopUV - ((baseSizeUV / 2.f) - (frameSizeUV / 2.f)) - offsetUV;
		float2 animUV = finalLeftTop + baseSizeUV * _in.UV;

		if (leftTopUV.x < animUV.x && animUV.x < (leftTopUV + frameSizeUV).x &&
			leftTopUV.y < animUV.y && animUV.y < (leftTopUV + frameSizeUV).y)
		{
			outColor = g_AnimAtlas.Sample(g_sampler_0, animUV);
		}
		else
		{
			clip(-1);
		}
		
	}
	else if (btex_0)
	{
		outColor = g_texture_0.Sample(g_sampler_0, _in.UV);
	}
	
	return outColor;
}

struct VERTEX_OUT_LIGHT
{
	float4 position : SV_Position;
	float3 worldPos : POSITION;
	float2 UV : TEXCOORD;
};

// Vertex Shader
VERTEX_OUT_LIGHT VS_Std2D_Light(VERTEX_IN _in)
{
	VERTEX_OUT_LIGHT output = (VERTEX_OUT_LIGHT)0.f;

	float4 worldPos = mul(float4(_in.pos, 1.f), g_worldMatrix);

	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.worldPos = worldPos.xyz;
	output.UV = _in.UV;

	return output;
}

// Pixel Shader
float4 PS_Std2D_Light(VERTEX_OUT_LIGHT _in) : SV_Target
{
	float4 outColor = float4(1.f, 0.f, 1.f, 1.f);

	if (bAnim2D)
	{
		float2 finalLeftTop = leftTopUV - ((baseSizeUV / 2.f) - (frameSizeUV / 2.f)) - offsetUV;
		float2 animUV = finalLeftTop + baseSizeUV * _in.UV;

		if (leftTopUV.x < animUV.x && animUV.x < (leftTopUV + frameSizeUV).x &&
			leftTopUV.y < animUV.y && animUV.y < (leftTopUV + frameSizeUV).y)
		{
			outColor = g_AnimAtlas.Sample(g_sampler_0, animUV);
		}
		else
		{
			clip(-1);
		}

	}
	else if (btex_0)
	{
		outColor = g_texture_0.Sample(g_sampler_0, _in.UV);
	}

	tLightColor finalColor = (tLightColor) 0.f;
	for (int i = 0; i < g_light2DCount; ++i)
	{
		float leng = length(g_Light2DBuffer[i].lightPos.xy - _in.worldPos.xy);
		float ratio = cos(saturate(leng / g_Light2DBuffer[i].range) * (3.1415922635f / 2.f));

		if (2 == g_Light2DBuffer[i].lightType)
		{
			float2 worldPosProj = dot(_in.worldPos, g_Light2DBuffer[i].lightDir.xyz) * g_Light2DBuffer[i].lightDir.xy;
			float2 lightPosProj = dot(g_Light2DBuffer[i].lightPos, g_Light2DBuffer[i].lightDir) * g_Light2DBuffer[i].lightDir.xy;
			float sign = dot(normalize(worldPosProj - lightPosProj), g_Light2DBuffer[i].lightDir.xy);
			float angle = acos((length(worldPosProj - lightPosProj) / leng));
			float angleRatio = cos(saturate((angle / g_Light2DBuffer[i].angle)) * (3.1415922635f / 2.f));

			finalColor.diffuse += g_Light2DBuffer[i].color.diffuse * ratio * angleRatio * sign;
		}
		else if (1 == g_Light2DBuffer[i].lightType)
		{
			finalColor.diffuse += g_Light2DBuffer[i].color.diffuse * ratio;
		}

	}

	outColor.xyz = outColor.xyz * finalColor.diffuse.xyz;

	return outColor;
}

VERTEX_OUT VS_Collider2D(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT)0.f;

	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);

	return output;
}

float4 PS_Collider2D(VERTEX_OUT _in) : SV_Target
{
	float4 pos = _in.position;
	if (g_int_0)
	{
		return float4(0.9f, 0.2f, 0.2f, 1.f);
	}
	
	if (g_int_1)
	{
		return float4(0.2f, 0.2f, 0.9f, 1.f);
	}

	return float4(0.2f, 0.9f, 0.2f, 1.f);
}

struct VERTEX_TILEMAP_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VERTEX_TILEMAP_OUT
{
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float2 uv : TEXCOORD;
};

// Vertex Shader
VERTEX_TILEMAP_OUT VS_TileMap(VERTEX_TILEMAP_IN _in)
{
	VERTEX_TILEMAP_OUT output = (VERTEX_TILEMAP_OUT)0.f;

	float4 worldPos = mul(float4(_in.pos, 1.f), g_worldMatrix);

	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.worldPos = worldPos.xyz;
	output.uv = _in.uv;

	return output;
}

// Pixel Shader
float4 PS_TileMap(VERTEX_TILEMAP_OUT _in) : SV_Target
{
	float2 originUV = float2(_in.uv.x * (float)g_int_0, _in.uv.y * (float)g_int_1);

	// 구조화버퍼 접근 인덱스

	float2 uv = g_TileBuffer[floor(originUV.y) * g_int_0 + floor(originUV.x)].leftTopUV + 
		float2(frac(originUV.x) * g_TileBuffer[floor(originUV.y) * g_int_0 + floor(originUV.x)].tileSizeUV.x, 
			frac(originUV.y) * g_TileBuffer[floor(originUV.y) * g_int_0 + floor(originUV.x)].tileSizeUV.y);

	float4 outColor = g_texture_0.Sample(g_sampler_1, uv);

	tLightColor finalColor = (tLightColor) 0.f;
	for (int i = 0; i < g_light2DCount; ++i)
	{
		float leng = length(g_Light2DBuffer[i].lightPos.xy - _in.worldPos.xy);
		float ratio = cos(saturate(leng / g_Light2DBuffer[i].range) * (3.1415922635f / 2.f));

		if (2 == g_Light2DBuffer[i].lightType)
		{
			float2 worldPosProj = dot(_in.worldPos, g_Light2DBuffer[i].lightDir.xyz) * g_Light2DBuffer[i].lightDir.xy;
			float2 lightPosProj = dot(g_Light2DBuffer[i].lightPos, g_Light2DBuffer[i].lightDir) * g_Light2DBuffer[i].lightDir.xy;
			float sign = dot(normalize(worldPosProj - lightPosProj), g_Light2DBuffer[i].lightDir.xy);
			float angle = acos((length(worldPosProj - lightPosProj) / leng));
			float angleRatio = cos(saturate((angle / g_Light2DBuffer[0].angle)) * (3.1415922635f / 2.f));

			finalColor.diffuse += g_Light2DBuffer[i].color.diffuse * ratio * angleRatio * sign;
		}
		else if (1 == g_Light2DBuffer[0].lightType)
		{
			finalColor.diffuse += g_Light2DBuffer[0].color.diffuse * ratio;
		}

	}

	outColor.xyz = outColor.xyz * finalColor.diffuse.xyz;

	return outColor;
}

#endif