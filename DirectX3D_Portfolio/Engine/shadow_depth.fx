#ifndef _SHADOWDEPTH
#define _SHADOWDEPTH

#include "value.fx"
#include "func.fx"

struct VS_ShadowIn
{
	float3 pos : POSITION;
	float4 weights : BLENDWEIGHT;
	float4 indices : BLENDINDICES;
};

struct VS_ShadowOut
{
	float4 pos : SV_Position;
	float4 projection : POSITION;
};

VS_ShadowOut VS_ShadowDepth(VS_ShadowIn _in)
{
	VS_ShadowOut output = (VS_ShadowOut) 0.f;
    
	if (g_anim)
	{
		Skinning(_in.pos, _in.weights, _in.indices, 0);
	}
    
	output.pos = mul(float4(_in.pos, 1.f), g_matrixWVP);
	output.projection = output.pos;

	return output;
}

struct VS_ShadowIn_Inst
{
	float3 pos : POSITION;
	float4 weights : BLENDWEIGHT;
	float4 indices : BLENDINDICES;
    
    // Instancing 당 데이터
	row_major matrix matrixWorld : WORLD;
	row_major matrix matrixWV : WV;
	row_major matrix matrixWVP : WVP;
    row_major matrix matrixVP : VP;
    row_major matrix matrixV : V;
	int rowIndex : ROWINDEX; // Animaion 행렬 행

	uint instanceID : SV_InstanceID;
};

#define BoneOffSet			g_int_0
#define HandBoneWorldMatrix	g_matrix_1
StructuredBuffer<matrix> g_BoneMatrixBuffer : register(t14);

VS_ShadowOut VS_ShadowDepth_Inst(VS_ShadowIn_Inst _in)
{
	VS_ShadowOut output = (VS_ShadowOut) 0.f;
    
    if (BoneOffSet)
    {
        float3 worldPos = mul(float4(_in.pos, 1.f), _in.matrixWorld).xyz;
        worldPos = mul(float4(worldPos.xyz, 1.f), g_BoneMatrixBuffer[0]).xyz;
        worldPos = mul(float4(worldPos.xyz, 1.f), HandBoneWorldMatrix).xyz;
     
        output.pos = mul(float4(worldPos.xyz, 1.f), _in.matrixVP);
        output.projection = output.pos;
    }
    else
    {
        if (g_anim)
        {
            Skinning(_in.pos, _in.weights, _in.indices, _in.rowIndex);
        }
		
        output.pos = mul(float4(_in.pos, 1.f), _in.matrixWVP);
        output.projection = output.pos;
    }
	
	return output;
}

float4 PS_ShadowDepth(VS_ShadowOut _input) : SV_Target
{
	return float4(_input.projection.z / _input.projection.w, 0.f, 0.f, 0.f);
}

#endif