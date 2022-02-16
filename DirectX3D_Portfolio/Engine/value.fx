#ifndef _VALUE
#define _VALUE

#include "struct.fx"

cbuffer TRANSFORM : register(b0)
{
	row_major Matrix g_worldMatrix;
	row_major Matrix g_viewMatrix;
	row_major Matrix g_projectionMatrix;
	
	row_major Matrix g_worldMatrixInverse;
	row_major Matrix g_viewMatrixInverse;
	row_major Matrix g_projectionMatrixInverse;

	row_major Matrix g_matrixWV;
	row_major Matrix g_matrixWVP;
	row_major Matrix g_matrixVP;
}

cbuffer MATERIAL_PARAM :register(b1)
{
	float4 diff;
	float4 spec;
	float4 amb;
	float4 emissive;

	int g_int_0;
	int g_int_1;
	int g_int_2;
	int g_int_3;

	float g_float_0;
	float g_float_1;
	float g_float_2;
	float g_float_3;

	float2 g_vec2_0;
	float2 g_vec2_1;
	float2 g_vec2_2;
	float2 g_vec2_3;

	float4 g_vec4_0;
	float4 g_vec4_1;
	float4 g_vec4_2;
	float4 g_vec4_3;

	row_major matrix g_matrix_0;
	row_major matrix g_matrix_1;
	row_major matrix g_matrix_2;
	row_major matrix g_matrix_3;

	int btex_0;
	int btex_1;
	int btex_2;
	int btex_3;
	int btex_4;
	int btex_5;
	int btex_6;
	int btex_7;
	int btex_arr_0;
	int btex_arr_1;
	int btex_cube_0;
	int btex_cube_1;
	
	int g_anim;
	int g_boneCount;
	int2 padding;
}

cbuffer ANIM2D : register(b2)
{
	float2 leftTopUV;
	float2 frameSizeUV;
	float2 offsetUV;
	float2 baseSizeUV;

	int bAnim2D;
	int3 anim2DPadding;
}

cbuffer GLOBAL_CONST : register(b3)
{
	float2 g_resolution;
	float2 g_noiseResolution;
	float g_deltaTime;
	float g_accTime;
	
	int g_light2DCount;
	int g_light3DCount;
}

Texture2D g_texture_0 : register(t0);
Texture2D g_texture_1 : register(t1);
Texture2D g_texture_2 : register(t2);
Texture2D g_texture_3 : register(t3);
Texture2D g_texture_4 : register(t4);
Texture2D g_texture_5 : register(t5);
Texture2D g_texture_6 : register(t6);
Texture2D g_texture_7 : register(t7);

Texture2DArray g_texture_arr_0 : register(t8);
Texture2DArray g_texture_arr_1 : register(t9);

TextureCube g_texture_cube_0 : register(t10);
TextureCube g_texture_cube_1 : register(t11);

StructuredBuffer<Matrix> g_arrBoneMatrix : register(t12);

Texture2D g_AnimAtlas : register(t50);
StructuredBuffer<tLightInfo> g_Light2DBuffer : register(t51);
StructuredBuffer<tLightInfo> g_Light3DBuffer : register(t52);
StructuredBuffer<tTileInfo> g_TileBuffer : register(t53);
Texture2D g_noise : register(t54);

SamplerState g_sampler_0 : register(s0);
SamplerState g_sampler_1 : register(s1);

#endif