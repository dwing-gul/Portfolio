#pragma once
#include "global.h"

struct tFBXMaterial
{
	tMaterialData material;
	wstring materialName;
	wstring diff;
	wstring normal;
	wstring spec;
};

struct tWeightsAndIndices
{
	int boneIndex;
	double weight;
};

struct tContainer
{
	wstring								name;
	wstring								parentName;
	vector<wstring>						vecChildName;
	bool								bMesh;
	Vec3								translate;
	Vec3								scale;
	Vec3								rotation;
	Vec3								meshLength;
	Vec3								meshCenter;
	vector<Vec3>						vecPos;
	vector<Vec3>						vecTangent;
	vector<Vec3>						vecBinormal;
	vector<Vec3>						vecNormal;
	vector<Vec2>						vecUV;

	vector<Vec4>						vecIndices;
	vector<Vec4>						vecWeights;

	vector<vector<UINT>>				vecIndex;
	vector<tFBXMaterial>				vecMaterial;

	// Animation 관련 정보
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWeightIndices;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWeightIndices.resize(_iSize);
	}
};

struct tKeyFrame
{
	FbxAMatrix  transformMatrix;
	double		time;
};

struct tBone
{
	wstring				boneName;
	int					depth;			// 계층구조 깊이
	int					parentIndex;	// 부모 Bone 의 인덱스
	FbxAMatrix			offsetMatrix;		// Offset 행렬( -> 뿌리 -> Local)
	FbxAMatrix			boneMatrix;
	vector<tKeyFrame>	vecKeyFrame;
};

struct tAnimClip
{
	wstring		name;
	FbxTime		startTime;
	FbxTime		endTime;
	FbxLongLong	timeLength;
	FbxTime::EMode mode;
};

class Mesh;

class FBXLoader
{
private:
	FbxManager*								m_manager;
	FbxScene*								m_scene;
	FbxImporter*							m_importer;

	vector<tContainer>						m_vecContainer;
	map<wstring, tContainer>				m_mapContainer;

	// Animation
	vector<tBone*>							m_vecBone;
	FbxArray<FbxString*>					m_arrAnimName;
	vector<tAnimClip*>						m_vecAnimClip;
	//모든 애니메이션의 프레임 행렬 정보를 담는 역할 vector를 3번 사용한 
	//이유는 애니메이션 클립별로 관리하기 위해서
	vector<vector<vector<tKeyFrame>>>		m_vecAllKeyFrame;
	vector<int>								m_vecAnimFrame;

	bool									m_bHasAnim;

public:
	void init();
	void LoadFbx(const wstring& _path);
	int GetContainerCount() { return (int)m_vecContainer.size(); }
	const tContainer& GetContainer(int _index) { return m_vecContainer[_index]; }
	vector<tBone*>& GetBones() { return m_vecBone; }
	vector<tAnimClip*>& GetAnimClip() { return m_vecAnimClip; }

	map<wstring, tContainer>& GetMapContainer() { return m_mapContainer; }
	void SetHasAnim(bool _anim) { m_bHasAnim = _anim; }

private:
	void LoadMeshDataFromNode(FbxNode* _node);
	void LoadMesh(FbxMesh* _fbxMesh);
	void LoadMaterial(FbxSurfaceMaterial* _materialSurface);

	void GetTangent(FbxMesh* _mesh, tContainer* _container, int _index, int _vertexOrder);
	void GetBinormal(FbxMesh* _mesh, tContainer* _container, int _index, int _vertexOrder);
	void GetNormal(FbxMesh* _mesh, tContainer* _container, int _index, int _vertexOrder);
	void GetUV(FbxMesh* _mesh, tContainer* _container, int _index, int _UVIndex);

	Vec4 GetMaterialData(FbxSurfaceMaterial* _surface, const char* _materialName, const char* _materialFactorName);
	wstring GetMaterialTextureName(FbxSurfaceMaterial* _surface, const char* _materialProperty);

	void LoadTexture();
	void CreateMaterial();

	// Animation
	void LoadSkeleton(FbxNode* _node);
	void LoadSkeleton_Re(FbxNode* _node, int _depth, int _index, int _parentIndex);
	//Clip
	void LoadAnimationClip();
	void Triangulate(FbxNode* _node);

	void LoadAnimationData(FbxMesh* _mesh, tContainer* _container);
	void LoadWeightsAndIndices(FbxCluster* _cluster, int _boneIndex, tContainer* _container);
	void LoadOffsetMatrix(FbxCluster* _cluster, const FbxAMatrix& _nodeTransformMatrix, int _boneIndex, tContainer* _container);
	void LoadKeyframeTransform(FbxNode* _node, FbxCluster* _cluster, const FbxAMatrix& _nodeTransformMatrix, int _boneIndex, tContainer* _container);

	int FindBoneIndex(string _boneName);
	FbxAMatrix GetTransform(FbxNode* _node);

	void CheckWeightAndIndices(FbxMesh* _mesh, tContainer* _container);

public:
	FBXLoader();
	~FBXLoader();
};

