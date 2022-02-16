#pragma once
#include "global.h"

struct tVertex
{
	Vec3 pos;
	Vec2 UV;

	Vec3 tangent;
	Vec3 normal;
	Vec3 binormal;

	Vec4 weights;
	Vec4 indices;

public :
	bool operator == (tVertex _vertex)
	{
		if (abs(pos.x - _vertex.pos.x) < 0.00001f && 
			abs(pos.y - _vertex.pos.y) < 0.00001f &&
			abs(pos.z - _vertex.pos.z) < 0.00001f &&
			UV == _vertex.UV && 
			tangent == _vertex.tangent && 
			normal == _vertex.normal && 
			binormal == _vertex.binormal &&
			weights == _vertex.weights && 
			indices == _vertex.indices)
		{
			return true;
		}

		return false;
	}

	bool operator != (tVertex _vertex)
	{
		return !((*this) == _vertex);
	}
};

struct tLightColor
{
	Vec4 diffuse;
	Vec4 specular;
	Vec4 ambient;
};

struct tEvent
{
	EVENT_TYPE type;
	DWORD_PTR lParam;
	DWORD_PTR wParam;
};

struct tRay
{
	Vec3 start;
	Vec3 dir;
};

struct tLightInfo
{
	tLightColor color;

	Vec4 lightPos;
	Vec4 lightDir;

	LIGHT_TYPE lightType;
	float range;
	float angle;
	int padding;
};

struct tParticle
{
	Vec3 worldPos;
	Vec3 worldDir;

	float curTime;
	float maxTime;
	int alive;
	int padding[3];
};

struct tFrameTransform
{
	Vec4	translate;
	Vec4	scale;
	Vec4	rot;
};

struct tMTKeyFrame
{
	double	time;
	int		frame;
	Vec3	translate;
	Vec3	scale;
	Vec4	rot;
};


struct tMTBone
{
	wstring				boneName;
	int					depth;
	int					parentIndex;
	Matrix				offsetMatrix;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix				boneMatrix;   // 이거 안씀
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring			animName;
	int				startFrame;
	int				endFrame;
	int				frameLength;

	double			startTime;
	double			endTime;
	double			timeLength;
	float			updateTime; // 이거 안씀

	FbxTime::EMode	mode;
};

union uInstID
{
	struct
	{
		UINT mesh;
		WORD material;
		WORD materialIndex;
	};

	ULONG64 ID;
};

class CGameObject;

struct tInstObj
{
	CGameObject* obj;
	UINT materialIndex;
};

struct tTransform
{
	Matrix worldMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;

	Matrix worldMatrixInverse;
	Matrix viewMatrixInverse;
	Matrix projectionMatrixInverse;

	Matrix matrixWV;
	Matrix matrixWVP;
	Matrix matrixVP;
};

struct tMaterialData
{
	Vec4 diffuse;
	Vec4 specular;
	Vec4 ambient;
	Vec4 emissive;
};

struct tMaterialParam
{
	tMaterialData material;

	int intArr[4];
	float floatArr[4];
	Vec2 vec2Arr[4];
	Vec4 vec4Arr[4];
	Matrix matrixArr[4];

	int bTex[TEXTURE_COUNT];
	int arrAnimData[4];
};

struct tAnimFrame
{
	Vec2 leftTop;
	Vec2 frameSize;
	Vec2 offset;
	float duration;
};

struct tAnim2DInfo
{
	Vec2 leftTopUV;
	Vec2 frameSizeUV;
	Vec2 offsetUV;
	Vec2 baseSizeUV;

	int bAnim2D;
	int padding[3];
};

struct tGlobalConst
{
	Vec2 resolution;
	Vec2 noiseResolution;
	float deltaTime;
	float accTime;
	int light2DCount;
	int light3DCount;
};

struct tTileInfo
{
	Vec2 leftTopUV;
	Vec2 tileSizeUV;
};