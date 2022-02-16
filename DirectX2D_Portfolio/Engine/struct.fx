#ifndef _STRUCT
#define _STRUCT


struct tLightColor
{
	float4 diffuse;
	float4 specular;
	float4 ambient;
};

struct tLightInfo
{
	tLightColor color;

	float4		lightPos;
	float4		axis;

	int			lightType;
	float		range;
	float		angle;
	int			padding;
};

struct tTileInfo
{
	float2 leftTopUV;
	float2 tileSizeUV;
};

struct tParticle
{
	float3 worldPos;
	float3 worldDir;

	float  curTime;
	float  maxTime;
	int	   alive;
	int3   padding;
};

struct tSharedData
{
	uint	spawnCount;
	uint	curCount;
	uint2	padding;
};

#endif