#ifndef _TERRAIN
#define _TERRAIN

#include "value.fx"
#include "func.fx"

#define HEIGHT_MAP g_texture_0
#define BRUSH_TEX g_texture_arr_0
#define TILE_TEX g_texture_arr_1

#define FACE_X g_int_0
#define FACE_Z g_int_1
#define BRUSH_INDEX g_int_2
#define BRUSH_VISIBLE g_int_3

#define TILE_COUNT g_float_0
#define MAX_TESS_LEVEL g_float_1

#define BRUSH_SCALE g_vec2_0
#define WEIGHTMAP_RESOLUTION g_vec2_1
#define TESS_DIST_MIN_MAX g_vec2_2

#define CAM_WORLD_POS g_vec4_0

StructuredBuffer<tRaycastOut> LOCATION : register(t13);
StructuredBuffer<float4> WEIGHT_MAP : register(t14);

struct VERTEX_IN
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
	
	float3 tangent : TANGENT;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
};

struct VERTEX_OUT
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
	float2 fullUV : TEXCOORD1;
};

VERTEX_OUT VS_Terrain(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	output.pos = _in.pos;
	output.UV = _in.UV;
	output.fullUV = _in.UV / float2(FACE_X, FACE_Z);
	return output;
}


struct PatchTessLevel
{
	float edge[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

PatchTessLevel PatchConstantFunc(InputPatch<VERTEX_OUT, 3> _patch, uint _patchID : SV_PrimitiveID)
{
	PatchTessLevel output = (PatchTessLevel) 0.f;
	
	float3 worldPos[3] =
	{
		float3(0.f, 0.f, 0.f),
		float3(0.f, 0.f, 0.f),
		float3(0.f, 0.f, 0.f)
	};
	
	for (int i = 0; i < 3; ++i)
	{
		worldPos[i] = mul(float4(_patch[i].pos, 1.f), g_worldMatrix).xyz;
	}
	
	float3 edgePos[3] =
	{
		(worldPos[1] + worldPos[2]) / 2.f,
		(worldPos[0] + worldPos[2]) / 2.f,
		(worldPos[0] + worldPos[1]) / 2.f
	};
	
	for (i = 0; i < 3; ++i)
	{
		output.edge[i] = CalTessLevel(CAM_WORLD_POS.xyz, edgePos[i], TESS_DIST_MIN_MAX[0], TESS_DIST_MIN_MAX[1], MAX_TESS_LEVEL);

	}
	
	float3 insideWorldPos = (worldPos[0] + worldPos[1] + worldPos[2]) / 3.f;
	
	output.inside = CalTessLevel(CAM_WORLD_POS.xyz, insideWorldPos, TESS_DIST_MIN_MAX[0], TESS_DIST_MIN_MAX[1], MAX_TESS_LEVEL);
	
	return output;
}

[domain("tri")]
[outputcontrolpoints(3)]
[outputtopology("triangle_cw")]
[partitioning("integer")]
[maxtessfactor(64.0)]
[patchconstantfunc("PatchConstantFunc")]
VERTEX_OUT HS_Terrain(InputPatch<VERTEX_OUT, 3> _patch, uint _vertexIndex : SV_OutputControlPointID, uint _patchID : SV_PrimitiveID)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;

	output.pos = _patch[_vertexIndex].pos;
	output.UV = _patch[_vertexIndex].UV;
	output.fullUV = _patch[_vertexIndex].fullUV;
	
	return output;
}

struct DS_OUT
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD;
	float2 fullUV : TEXCOORD1;
	float3 viewPos : POSITION;
	
	float3 viewTangent : TANGENT;
	float3 viewNormal : NORMAL;
	float3 viewBinormal : BINORMAL;
};

[domain("tri")]
DS_OUT DS_Terrain(OutputPatch<VERTEX_OUT, 3> _patch, PatchTessLevel _patchTessLevel, float3 _UV : SV_DomainLocation)
{
	DS_OUT output = (DS_OUT) 0.f;
	
	float3 localPos = _patch[0].pos * _UV[0] + _patch[1].pos * _UV[1] + _patch[2].pos * _UV[2];
	output.uv = _patch[0].UV * _UV[0] + _patch[1].UV * _UV[1] + _patch[2].UV * _UV[2];
	output.fullUV = _patch[0].fullUV * _UV[0] + _patch[1].fullUV * _UV[1] + _patch[2].fullUV * _UV[2];
	
	float localHeight = HEIGHT_MAP.SampleLevel(g_sampler_0, output.fullUV, 0).x;
	localPos.y = localHeight;
	output.viewPos = mul(float4(localPos, 1.f), g_matrixWV).xyz;
	
	float insideTess = _patchTessLevel.inside;
	
	float2 localLeftUV = float2(output.fullUV.x - (1.f / (FACE_X * insideTess)), output.fullUV.y);
	float2 localRightUV = float2(output.fullUV.x + (1.f / (FACE_X * insideTess)), output.fullUV.y);
	float2 localUpUV = float2(output.fullUV.x, output.fullUV.y - (1.f / (FACE_Z * insideTess)));
	float2 localDownUV = float2(output.fullUV.x, output.fullUV.y + (1.f / (FACE_Z * insideTess)));
	
	float3 localLeftPos = float3(localPos.x - (1.f / insideTess), HEIGHT_MAP.SampleLevel(g_sampler_0, localLeftUV, 0).x, localPos.z);
	float3 localRightPos = float3(localPos.x + (1.f / insideTess), HEIGHT_MAP.SampleLevel(g_sampler_0, localRightUV, 0).x, localPos.z);
	float3 localUpPos = float3(localPos.x, HEIGHT_MAP.SampleLevel(g_sampler_0, localUpUV, 0).x, localPos.z + (1.f / insideTess));
	float3 localDownPos = float3(localPos.x, HEIGHT_MAP.SampleLevel(g_sampler_0, localDownUV, 0).x, localPos.z - (1.f / insideTess));
	
	float3 viewLeftPos = mul(float4(localLeftPos, 1.f), g_matrixWV).xyz;
	float3 viewRightPos = mul(float4(localRightPos, 1.f), g_matrixWV).xyz;
	float3 viewUpPos = mul(float4(localUpPos, 1.f), g_matrixWV).xyz;
	float3 viewDownPos = mul(float4(localDownPos, 1.f), g_matrixWV).xyz;
	
	output.viewTangent = normalize(viewRightPos - viewLeftPos);
	output.viewBinormal = normalize(viewDownPos - viewUpPos);
	output.viewNormal = normalize(cross(output.viewTangent, output.viewBinormal));
	
	output.position = mul(float4(localPos, 1.f), g_matrixWVP);
	
	return output;
}

struct PS_OUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
};

PS_OUT PS_Terrain(DS_OUT _in)
{
	PS_OUT psout = (PS_OUT) 0.f;
	
	psout.color = float4(0.8f, 0.8f, 0.8f, 0.f);
	psout.position.xyz = _in.viewPos;
	
	float3 viewNormal = _in.viewNormal;
	
	if (btex_arr_1)
	{
		float2 derivX = ddx(_in.uv);
		float2 derivY = ddy(_in.uv);
		
		int2 weightIndex = (int2) (_in.fullUV * WEIGHTMAP_RESOLUTION);
		float4 weight = WEIGHT_MAP[weightIndex.y * (int) WEIGHTMAP_RESOLUTION.x + weightIndex.x];
		float4 color = (float4) 0.f;
		
		int maxWeightIndex = 0;
		float maxWeight = 0.f;
		
		for (int i = 0; i < 4; ++i)
		{
			color += TILE_TEX.SampleGrad(g_sampler_0, float3(_in.uv, i), derivX, derivY) * weight[i];
			
			if (maxWeight < weight[i])
			{
				maxWeight = weight[i];
				maxWeightIndex = i;
			}
		}
		
		psout.color.rgb = color.rgb;
		float3 tangentSpaceNormal = TILE_TEX.SampleGrad(g_sampler_0, float3(_in.uv, maxWeightIndex + TILE_COUNT), derivX, derivY).xyz;
		tangentSpaceNormal = tangentSpaceNormal * 2.f - 1.f;

		float3x3 matrixTBN = { _in.viewTangent, _in.viewBinormal, _in.viewNormal };
		viewNormal = mul(tangentSpaceNormal, matrixTBN);
	}
	
	psout.normal.xyz = viewNormal;
	
	if (BRUSH_VISIBLE)
	{
		float2 diff = ((LOCATION[0].UV - _in.fullUV) / BRUSH_SCALE);
		float2 brushUV = float2(0.5f, 0.5f) - diff;
		
		if (0.f <= brushUV.x && brushUV.x <= 1.f &&
			0.f <= brushUV.y && brushUV.y <= 1.f)
		{
			float4 brushColor = BRUSH_TEX.Sample(g_sampler_0, float3(brushUV, BRUSH_INDEX));
			psout.color.a = brushColor.a;
		}

	}
	
	return psout;
}
#endif