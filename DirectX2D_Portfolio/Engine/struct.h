#pragma once
#include "global.h"

struct tVertex
{
	Vector3 pos;
	Vector2 uv;

	Vector3 tangent;
	Vector3 normal;
	Vector3 binormal;
};

struct tLightColor
{
	Vector4 diffuse;
	Vector4 specular;
	Vector4 ambient;
};

struct tEvent
{
	EVENT_TYPE type;
	DWORD_PTR  lparam;
	DWORD_PTR  wparam;
	DWORD_PTR  eparam;
};

struct tLightInfo
{
	tLightColor color;

	Vector4		lightPos;
	Vector4		axis;

	LIGHT_TYPE	lightType;
	float		range;
	float		angle;
	int			padding;
};

struct tParticle
{
	Vector3 worldPos;
	Vector3 worldDir;

	float	curTime;
	float	maxTime;
	int		alive;
	int		padding[3];
};

struct tTransform
{
	Matrix worldMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;

	Matrix matrixWV;
	Matrix matrixWVP;
};

struct tMaterialData
{
	Vector4 diff;
	Vector4 spec;
	Vector4 amb;
};

struct tMaterialParam
{
	tMaterialData material;

	int			  intArr[4];
	float		  floatArr[4];
	Vector2		  vector2Arr[4];
	Vector4		  vector4Arr[4];
	Matrix		  matrixArr[4];

	int			  bTex[(UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0];
};

struct tAnim2DInfo
{
	Vector2 leftTopUV;
	Vector2 frameSizeUV;
	Vector2 offsetUV;
	Vector2 baseSizeUV;

	int		bAnim2D;
	int		padding[3];
};

struct tGlobalConst
{
	Vector2 resolution;
	Vector2 noiseResolution;
	float	deltaTime;
	float	accTime;
	int		light2DCount;
	int		light3DCount;
};

struct tTileInfo
{
	Vector2 leftTopUV;
	Vector2 tileSizeUV;
};
