#ifndef _EFFECT
#define _EFFECT

#include "value.fx"
#include "func.fx"

struct VERTEX2D_IN
{
    float3 pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VERTEX2D_OUT
{
    float4 position : SV_Position;
    float2 UV : TEXCOORD;
};


struct VERTEX3D_IN
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
    uint rowIndex : ROWINDEX;
};

struct VERTEX3D_OUT
{
    float4 position : SV_Position;
    float2 UV : TEXCOORD;
    float3 viewPos : POSITION;
	
    float3 viewTangent : TANGENT;
    float3 viewNormal : NORMAL;
    float3 viewBinormal : BINORMAL;
};

// Vertex Shader
VERTEX2D_OUT VS_Std2D(VERTEX2D_IN _in)
{
    VERTEX2D_OUT output = (VERTEX2D_OUT) 0.f;

    float4 projectionPos = mul(float4(_in.pos, 1.f), g_matrixWVP);

    output.position = projectionPos;
    output.UV = _in.UV;

    return output;
}

#define EffectColor	g_vec4_0
// Pixel Shader
float4 PS_Effect2D(VERTEX2D_OUT _in) : SV_Target
{
    float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
	
    if (bAnim2D)
    {
        float2 finalLeftTop = leftTopUV - ((baseSizeUV / 2.f) - (frameSizeUV / 2.f)) - offsetUV;
        float2 animUV = finalLeftTop + baseSizeUV * _in.UV;

        if (leftTopUV.x < animUV.x && animUV.x < (leftTopUV + frameSizeUV).x &&
			leftTopUV.y < animUV.y && animUV.y < (leftTopUV + frameSizeUV).y)
        {
            outColor = g_AnimAtlas.Sample(g_sampler_0, animUV);
            outColor.a = g_AnimAtlas.Sample(g_sampler_0, animUV).x;
            outColor.xyz *= EffectColor.xyz;
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
    else
    {
        outColor.xyz = EffectColor.xyz;
    }
	
    return outColor;
}

float4 PS_Effect2D_Coverage(VERTEX2D_OUT _in) : SV_Target
{
    float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
	
    if (bAnim2D)
    {
        float2 finalLeftTop = leftTopUV - ((baseSizeUV / 2.f) - (frameSizeUV / 2.f)) - offsetUV;
        float2 animUV = finalLeftTop + baseSizeUV * _in.UV;

        if (leftTopUV.x < animUV.x && animUV.x < (leftTopUV + frameSizeUV).x &&
			leftTopUV.y < animUV.y && animUV.y < (leftTopUV + frameSizeUV).y)
        {
            outColor = g_AnimAtlas.Sample(g_sampler_0, animUV);
            outColor.a = g_AnimAtlas.Sample(g_sampler_0, animUV).x;
            outColor.xyz *= EffectColor.xyz;
        }
        else
        {
            clip(-1);
        }
    }
    else if (btex_0)
    {
        outColor = g_texture_0.Sample(g_sampler_0, _in.UV);
		
        if (EffectColor.x != 0.f || EffectColor.y != 0.f || EffectColor.z != 0.f)
            outColor.xyz += EffectColor.xyz;
        //outColor.a = g_texture_0.Sample(g_sampler_0, _in.UV).x;
    }
	
    return outColor;
}

#define Mod         g_int_0
#define AlphaRatio	g_float_0

float4 PS_BuffEffect2D_Coverage(VERTEX2D_OUT _in) : SV_Target
{
    float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
    
    if (btex_0)
    {
        outColor.a = g_texture_0.Sample(g_sampler_0, _in.UV).x;
        
        if (outColor.a == 0.f)
            clip(-1);
		
        if (EffectColor.x != 0.f || EffectColor.y != 0.f || EffectColor.z != 0.f)
            outColor.xyz = EffectColor.xyz;
        
        if (AlphaRatio != 0.f)
        {
            if (Mod == 1)
            {
                outColor.a -= AlphaRatio;
            }
            else
            {
                outColor.a += AlphaRatio;
            }
        }
       
    }
	
    return outColor;
}

// 아야 궁 라인 용
float4 PS_Effect2D_Line(VERTEX2D_OUT _in) : SV_Target
{
    float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
    
    if (btex_0)
    {
        outColor.a = g_texture_0.Sample(g_sampler_0, _in.UV).x;
        outColor.xyz = EffectColor.xyz;
    }
    else
    {
        outColor.xyz = EffectColor.xyz;
        if (_in.UV.x >= 0.9f || _in.UV.y >= 0.9f)
            outColor.a = 1.f;
        else
            clip(-1);

    }
    
    return outColor;
}

#define Color	g_vec4_0
#define EffectState	g_int_0
#define AlphaRatio	g_float_0

float4 PS_Effect2D_Decal(VERTEX2D_OUT _in) : SV_Target
{
    float4 outColor = float4(1.f, 1.f, 1.f, 1.f);
	
    if (btex_0)
    {
        outColor = g_texture_0.Sample(g_sampler_0, _in.UV);
        
        if (EffectState == 1)
        {
            outColor.xyz = Color.xyz;
        }
        else if (EffectState == 2)
        {
            outColor.xyz += Color.xyz;
            outColor.a *= AlphaRatio;
        }
    }
       
    return outColor;
}

//3d Effect
#define OutputTexture       g_texture_0
#define NormalMapTexture    g_texture_1
#define SpecularTexture     g_texture_2
#define SkyBox              g_texture_cube_0

StructuredBuffer<matrix> g_BoneMatrixBuffer : register(t14);

#define BoneOffSet			g_int_0
#define HandBoneWorldMatrix	g_matrix_1

VERTEX3D_OUT VS_Effect3D(VERTEX3D_IN _in)
{
    VERTEX3D_OUT output = (VERTEX3D_OUT) 0.f;
    
    if (g_anim)
    {
        Skinning(_in.pos, _in.tangent, _in.binormal, _in.normal, _in.weights, _in.indices, 0);
    }
		
    output.position = mul(float4(_in.pos, 1.f), g_matrixWVP);
    output.viewPos = mul(float4(_in.pos, 1.f), g_matrixWV).xyz;
	
    output.UV = _in.UV;
	
    output.viewTangent = normalize(mul(float4(_in.tangent, 0.f), g_matrixWV).xyz);
    output.viewNormal = normalize(mul(float4(_in.normal, 0.f), g_matrixWV).xyz);
    output.viewBinormal = normalize(mul(float4(_in.binormal, 0.f), g_matrixWV).xyz);

    return output;
}

VERTEX3D_OUT VS_Effect3D_Inst(VERTEX3D_IN _in)
{
    VERTEX3D_OUT output = (VERTEX3D_OUT) 0.f;
	
    if (g_anim)
    {
        Skinning(_in.pos, _in.tangent, _in.binormal, _in.normal, _in.weights, _in.indices, _in.rowIndex);
    }
	
    output.position = mul(float4(_in.pos, 1.f), _in.matrixWVP);
    output.UV = _in.UV;
	
    output.viewPos = mul(float4(_in.pos, 1.f), _in.matrixWV).xyz;
	
    output.viewTangent = normalize(mul(float4(_in.tangent, 0.f), _in.matrixWV).xyz);
    output.viewNormal = normalize(mul(float4(_in.normal, 0.f), _in.matrixWV).xyz);
    output.viewBinormal = normalize(mul(float4(_in.binormal, 0.f), _in.matrixWV).xyz);

    return output;
}

#define DiffuseColor    g_vec4_0
#define AlphaRatio      g_float_0 

float4 PS_Effect3D_Default(VERTEX3D_OUT _in) : SV_Target
{
    float4 psout = (float4) 0.f;
    
    psout = float4(1.f, 1.f, 1.f, 1.f);
	
    
    float3 viewNormal = _in.viewNormal;
	
    if (btex_0)
    {
        float4 sampleColor = OutputTexture.Sample(g_sampler_0, _in.UV);
		
        psout = DiffuseColor;

    }
    else
    {
        if (DiffuseColor.x != 0.f || DiffuseColor.y != 0.f
		|| DiffuseColor.z != 0.f || DiffuseColor.w != 0.f)
        {
            psout = DiffuseColor;
            
            if (AlphaRatio != 0.f)
            {
                psout.a = AlphaRatio;
            }

        }
    }
	
    //노말 텍스쳐
    if (btex_1)
    {
        float3 tangentSpaceNormal = NormalMapTexture.Sample(g_sampler_0, _in.UV).xyz;
        tangentSpaceNormal = tangentSpaceNormal * 2.f - 1.f;

        float3x3 matrixTBN = { _in.viewTangent, _in.viewBinormal, _in.viewNormal };
        viewNormal = mul(tangentSpaceNormal, matrixTBN);
    }
	
    //Specular
    if (btex_2)
    {
        psout *= SpecularTexture.Sample(g_sampler_1, _in.UV);
    }
	
    
    
    return psout;
}


#endif