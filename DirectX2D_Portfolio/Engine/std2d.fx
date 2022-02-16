#ifndef _STD2D
#define _STD2D
#include "value.fx"

#define ANISOTROPIC g_sampler_0
#define MINIMUM_MIN_MAG_MIP_POINT g_sampler_1

struct VERTEX_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VERTEX_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VERTEX_OUT VS_Std2D(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT)0.f;
	
	float4 projectionPos = mul(float4(_in.pos, 1.f), g_matrixWVP);

	output.position = projectionPos;
	output.uv = _in.uv;

	return output;
}

float4 PS_Std2D(VERTEX_OUT _in) : SV_Target
{
	float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
	
	if (bAnim2D)
	{
		float2 finalLeftTop = leftTopUV - ((baseSizeUV / 2.f) - (frameSizeUV / 2.f)) - offsetUV;
		float2 animUV = finalLeftTop + baseSizeUV * _in.uv;

		if (leftTopUV.x < animUV.x && animUV.x < (leftTopUV + frameSizeUV).x &&
			leftTopUV.y < animUV.y && animUV.y < (leftTopUV + frameSizeUV).y)
		{
			outColor = g_AnimAtlas.Sample(ANISOTROPIC, animUV);
		}
		else
		{
			clip(-1);
		}
	}
	else if (bTex_0)
	{
		outColor = g_texture_0.Sample(ANISOTROPIC, _in.uv);
	}

	return outColor;
}

struct VERTEX_OUT_LIGHT
{
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float2 uv : TEXCOORD;
};

VERTEX_OUT_LIGHT VS_Std2D_Light(VERTEX_IN _in)
{
	VERTEX_OUT_LIGHT output = (VERTEX_OUT_LIGHT)0.f;

	float4 worldPos = mul(float4(_in.pos, 1.f), g_worldMatrix);
	float4 projectionPos = mul(float4(_in.pos, 1.f), g_matrixWVP);

	output.position = projectionPos;
	output.worldPos = worldPos.xyz;
	output.uv = _in.uv;

	return output;
}

float4 PS_Std2D_Light(VERTEX_OUT_LIGHT _in) : SV_Target
{
	float4 outColor = float4(1.f, 0.f, 1.f, 1.f);

	if (bAnim2D)
	{
		float2 finalLeftTop = leftTopUV - ((baseSizeUV / 2.f) - (frameSizeUV / 2.f)) - offsetUV;
		float2 animUV = finalLeftTop + baseSizeUV * _in.uv;

		if (leftTopUV.x < animUV.x && animUV.x < (leftTopUV + frameSizeUV).x &&
			leftTopUV.y < animUV.y && animUV.y < (leftTopUV + frameSizeUV).y)
		{
			outColor = g_AnimAtlas.Sample(ANISOTROPIC, animUV);
		}
		else
		{
			clip(-1);
		}

	}
	else if (bTex_0)
	{
		outColor = g_texture_0.Sample(ANISOTROPIC, _in.uv);
	}

	tLightColor finalColor = (tLightColor)0.f;
	for (int i = 0; i < g_light2DCount; ++i)
	{
		float leng = length(g_Light2DBuffer[i].lightPos.xy - _in.worldPos.xy);
		float ratio = cos(saturate(leng / g_Light2DBuffer[i].range) * (3.1415926535f / 2.f));

		if (2 == g_Light2DBuffer[i].lightType)
		{
			float2 worldPosProj = dot(_in.worldPos, g_Light2DBuffer[i].axis.xyz) * g_Light2DBuffer[i].axis.xy;
			float2 lightPosProj = dot(g_Light2DBuffer[i].lightPos.xy, g_Light2DBuffer[i].axis.xy) * g_Light2DBuffer[i].axis.xy;
			float sign = dot(normalize(worldPosProj - lightPosProj), g_Light2DBuffer[i].axis.xy);
			float angle = acos((length(worldPosProj - lightPosProj) / leng));
			float angleRatio = cos(saturate((angle / g_Light2DBuffer[i].angle)) * (3.1415926535f / 2.f));

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

#define COLLISION g_int_0

VERTEX_OUT VS_Collider2D(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT)0.f;

	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);

	return output;
}

float4 PS_Collider2D(VERTEX_OUT _in) : SV_Target
{
	float4 pos = _in.position;
	if (COLLISION)
	{
		return float4(0.9f, 0.2f, 0.2f, 1.f);
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
	float2 uv : TEXCOORD;
};

#define X_FACE_COUNT g_int_0
#define Y_FACE_COUNT g_int_1
#define ATLAS_TEXTURE g_texture_0

// Vertex Shader
VERTEX_TILEMAP_OUT VS_TileMap(VERTEX_TILEMAP_IN _in)
{
	VERTEX_TILEMAP_OUT output = (VERTEX_TILEMAP_OUT)0.f;

	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.uv = _in.uv;

	return output;
}

// Pixel Shader
float4 PS_TileMap(VERTEX_TILEMAP_OUT _in) : SV_Target
{
	float2 originUV = float2(_in.uv.x * (float)X_FACE_COUNT, _in.uv.y * (float)Y_FACE_COUNT);

	// 구조화버퍼 접근 인덱스
	float2 uv = g_TileBuffer[floor(originUV.y) * X_FACE_COUNT + floor(originUV.x)].leftTopUV +
		float2(frac(originUV.x) * g_TileBuffer[floor(originUV.y) * X_FACE_COUNT + floor(originUV.x)].tileSizeUV.x,
			frac(originUV.y) * g_TileBuffer[floor(originUV.y) * X_FACE_COUNT + floor(originUV.x)].tileSizeUV.y);

	float4 outColor = ATLAS_TEXTURE.Sample(MINIMUM_MIN_MAG_MIP_POINT, uv);

	return outColor;
}

struct VERTEX_TILEMAP_OUT_LIGHT
{
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float2 uv : TEXCOORD;
};

VERTEX_TILEMAP_OUT_LIGHT VS_TileMap_Light(VERTEX_TILEMAP_IN _in)
{
	VERTEX_TILEMAP_OUT_LIGHT output = (VERTEX_TILEMAP_OUT_LIGHT)0.f;

	float4 worldPos = mul(float4(_in.pos, 1.f), g_worldMatrix);

	output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.worldPos = worldPos.xyz;
	output.uv = _in.uv;

	return output;
}

// Pixel Shader
float4 PS_TileMap_Light(VERTEX_TILEMAP_OUT_LIGHT _in) : SV_Target
{
	float2 originUV = float2(_in.uv.x * (float)X_FACE_COUNT, _in.uv.y * (float)Y_FACE_COUNT);

	// 구조화버퍼 접근 인덱스
	float2 uv = g_TileBuffer[floor(originUV.y) * X_FACE_COUNT + floor(originUV.x)].leftTopUV +
		float2(frac(originUV.x) * g_TileBuffer[floor(originUV.y) * X_FACE_COUNT + floor(originUV.x)].tileSizeUV.x,
			frac(originUV.y) * g_TileBuffer[floor(originUV.y) * X_FACE_COUNT + floor(originUV.x)].tileSizeUV.y);

	float4 outColor = ATLAS_TEXTURE.Sample(MINIMUM_MIN_MAG_MIP_POINT, uv);

	tLightColor finalColor = (tLightColor)0.f;
	for (int i = 0; i < g_light2DCount; ++i)
	{
		float leng = length(g_Light2DBuffer[i].lightPos.xy - _in.worldPos.xy);
		float ratio = cos(saturate(leng / g_Light2DBuffer[i].range) * (3.1415922635f / 2.f));

		if (2 == g_Light2DBuffer[i].lightType)
		{
			float2 worldPosProj = dot(_in.worldPos, g_Light2DBuffer[i].axis.xyz) * g_Light2DBuffer[i].axis.xy;
			float2 lightPosProj = dot(g_Light2DBuffer[i].lightPos.xy, g_Light2DBuffer[i].axis.xy) * g_Light2DBuffer[i].axis.xy;
			float sign = dot(normalize(worldPosProj - lightPosProj), g_Light2DBuffer[i].axis.xy);
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
