#ifndef _STD3D_DEFERRED
#define _STD3D_DEFERRED

#include "value.fx"
#include "func.fx"

#define OutputTexture g_texture_0
#define NormalMapTexture g_texture_1
#define SpecularTexture g_texture_2
#define SkyBox g_texture_cube_0

struct VERTEX_IN
{
	float3 pos : POSITION;
	float2 UV : TEXCOORD;
	
	float3 tangent : TANGENT;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	
	float4 weights : BLENDWEIGHT;
	float4 indices : BLENDINDICES;
	
	 // Per Instance Data
	row_major matrix worldMatrix : WORLD;
	row_major matrix matrixWV : WV;
	row_major matrix matrixWVP : WVP;
    row_major matrix matrixVP : VP;
    row_major matrix matrixV : V;
	uint rowIndex : ROWINDEX;
};

struct VERTEX_OUT
{
	float4 position : SV_Position;
	float2 UV : TEXCOORD;
	float3 viewPos : POSITION;
	
	float3 viewTangent : TANGENT;
	float3 viewNormal : NORMAL;
	float3 viewBinormal : BINORMAL;
};

StructuredBuffer<matrix> g_BoneMatrixBuffer : register(t14);

#define BoneOffSet			g_int_0
#define HandBoneWorldMatrix	g_matrix_1

VERTEX_OUT VS_Std3D_Deferred(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
	
    if (BoneOffSet)
    {
        float3 worldPos = mul(float4(_in.pos, 1.f), g_worldMatrix).xyz;
        worldPos = mul(float4(worldPos.xyz, 1.f), g_BoneMatrixBuffer[0]).xyz;
        worldPos = mul(float4(worldPos.xyz, 1.f), HandBoneWorldMatrix).xyz;
     
        output.position = mul(float4(worldPos.xyz, 1.f), g_matrixVP);
        output.viewPos = mul(float4(worldPos.xyz, 1.f), g_viewMatrix).xyz;
    }
	else
    {
        if (g_anim)
        {
            Skinning(_in.pos, _in.tangent, _in.binormal, _in.normal, _in.weights, _in.indices, 0);
        }
		
        output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
        output.viewPos = mul(float4(_in.pos, 1.f), g_matrixWV).xyz;
    }
	
    output.UV = _in.UV;
	
	output.viewTangent = normalize(mul(float4(_in.tangent, 0.f), g_matrixWV).xyz);
	output.viewNormal = normalize(mul(float4(_in.normal, 0.f), g_matrixWV).xyz);
	output.viewBinormal = normalize(mul(float4(_in.binormal, 0.f), g_matrixWV).xyz);

	return output;
}

VERTEX_OUT VS_Std3D_Deferred_Inst(VERTEX_IN _in)
{
	VERTEX_OUT output = (VERTEX_OUT) 0.f;
	
    if (BoneOffSet)
    {
        float3 worldPos = mul(float4(_in.pos, 1.f), _in.worldMatrix).xyz;
        worldPos = mul(float4(worldPos.xyz, 1.f), g_BoneMatrixBuffer[0]).xyz;
        worldPos = mul(float4(worldPos.xyz, 1.f), HandBoneWorldMatrix).xyz;
     
        output.position = mul(float4(worldPos.xyz, 1.f), _in.matrixVP);
        output.viewPos = mul(float4(worldPos.xyz, 1.f), _in.matrixV).xyz;
    }
    else
    {
        if (g_anim)
        {
            Skinning(_in.pos, _in.tangent, _in.binormal, _in.normal, _in.weights, _in.indices, _in.rowIndex);
        }
		
        output.position = mul(float4(_in.pos, 1.f), _in.matrixWVP);
        output.viewPos = mul(float4(_in.pos, 1.f), _in.matrixWV).xyz;
    }
	output.UV = _in.UV;
	
	output.viewTangent = normalize(mul(float4(_in.tangent, 0.f), _in.matrixWV).xyz);
	output.viewNormal = normalize(mul(float4(_in.normal, 0.f), _in.matrixWV).xyz);
	output.viewBinormal = normalize(mul(float4(_in.binormal, 0.f), _in.matrixWV).xyz);

	return output;
}

struct PS_OUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
};

#define DiffuseColor g_vec4_0

PS_OUT PS_Std3D_Deferred(VERTEX_OUT _in)
{
	PS_OUT psout = (PS_OUT) 0.f;
	
	psout.color = float4(1.f, 1.f, 1.f, 1.f);
	psout.position.xyz = _in.viewPos;
	
	float3 viewNormal = _in.viewNormal;
	
	if (btex_0)
	{
		float4 sampleColor = OutputTexture.Sample(g_sampler_1, _in.UV);
		
		psout.color.r = encode(sampleColor);

	}
    else
    {
        if (DiffuseColor.x != 0 || DiffuseColor.y != 0
		|| DiffuseColor.z != 0 || DiffuseColor.w != 0)
        {
            float4 sampleColor = g_vec4_0;
            psout.color.r = encode(sampleColor);
        }
    }
	
	if (btex_1)
	{
		float3 tangentSpaceNormal = NormalMapTexture.Sample(g_sampler_0, _in.UV).xyz;
		tangentSpaceNormal = tangentSpaceNormal * 2.f - 1.f;

		float3x3 matrixTBN = { _in.viewTangent, _in.viewBinormal, _in.viewNormal };
		viewNormal = mul(tangentSpaceNormal, matrixTBN);
	}
	
	if (btex_2)
	{
		float4 specularCoefficient = SpecularTexture.Sample(g_sampler_1, _in.UV);
		psout.color.g = encode(specularCoefficient);
	}
	
	psout.normal.xyz = viewNormal;
	
	if (btex_cube_0)
	{
		float3 eye = normalize(_in.viewPos);
		float3 eyeReflect = normalize(reflect(eye, viewNormal));
		float4 reflectColor = SkyBox.Sample(g_sampler_0, eyeReflect);
		
		psout.color *= reflectColor;
	}
	
	return psout;
}

#endif