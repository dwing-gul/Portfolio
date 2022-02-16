#pragma once
#include "CRes.h"
#include "CFBXLoader.h"

struct tIndexInfo
{
    ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC indexBufferDesc;
    UINT indexCount;
    void* indexSysMem;
};

class CMesh :
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_vertexBuffer;
    void* m_vertexSys;
    D3D11_BUFFER_DESC       m_vertexDesc;
    int m_vertexCount;

    vector<tIndexInfo>      m_vecIndexInfo;

    Vec3                    m_centerPos;
    Vec3                    m_xyzLength;

    vector<tMTAnimClip>     m_vecAnimClip;
    vector<tMTBone>         m_vecBones;

    CStructuredBuffer*      m_boneFrameData;
    CStructuredBuffer*      m_boneOffset;

public:
    static CMesh* CreateFromContainer(FBXLoader& _loader , int index = 0);

    virtual int Load(const wstring& _filePath);
    void Create(void* _vertexSys, UINT _vertexBufferSize, void* _indexSys, UINT _indexBufferSize);
    virtual void Save(const wstring& _relativePath);

    virtual void UpdateData(UINT _subset);
    void UpdateData_Inst(UINT _subset);
    void render(UINT _subset);
    void render_particle(UINT _count);
    void render_instancing(UINT _subset);
    void Reset(void* _vertexSys, UINT _vertexBufferSize, void* _indexSys, UINT _indexBufferSize);

    void* GetVertexSysMem() { return m_vertexSys; }
    int GetVertexCount() { return m_vertexCount; }
    UINT GetSubsetCount() { return (UINT)m_vecIndexInfo.size(); }
    const vector<tIndexInfo>& GetIndexInfo() { return m_vecIndexInfo; }
    const vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    Vec3 GetMeshCenterPos() { return m_centerPos; }
    Vec3 GetMeshXYZLength() { return m_xyzLength; }
    bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_boneFrameData; }
    CStructuredBuffer* GetBoneOffsetBuffer() { return m_boneOffset; }

    void SetMeshCenterPos(Vec3 _pos) { m_centerPos = _pos; }
    void SetMeshXYZLength(Vec3 _leng) { m_xyzLength = _leng; }

    CLONE_DISABLE(CMesh);

public:
    CMesh();
    CMesh(bool _bDefault);
    virtual ~CMesh();
};