#pragma once
#include "CRes.h"
#include "CFBXLoader.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;

class CMeshData :
    public CRes
{
private:
    Ptr<CMesh>                      m_mesh;
    vector<Ptr<CMaterial>>          m_vecMaterial;
    vector<wstring>                 m_vecParentName;
    vector<vector<wstring>>         m_vecChildName;
    vector<bool>                    m_vecBoolMesh;
    vector<tFrameTransform>         m_vecTransInformation;

public:
    static CMeshData* LoadFromFBX(const wstring& _filePath, bool _bHasAnim = false);
    static vector<CMeshData*>& LoadFromFBX_vector(const wstring& _filePath, bool _bHasAnim = false);

    Vec4 GetTranslate(UINT _index) { return m_vecTransInformation[_index].translate; }
    Vec4 GetScale(UINT _index) { return m_vecTransInformation[_index].scale; }
    Vec4 GetRotation(UINT _index) { return m_vecTransInformation[_index].rot; }

    void SetTransform(UINT _index, tFrameTransform _transform) { m_vecTransInformation[_index] = _transform; }

    void AddTransInformation(tFrameTransform _tranform) { m_vecTransInformation.push_back(_tranform); }

    virtual void Save(const wstring& _relativePath);
    virtual int Load(const wstring& _filePath);

    CGameObject* Instantiate();
    void InstantiateAndCreate(float _boundingRange = 100.f, int _layerIndex = 0);
    vector<tFrameTransform>& GetVectorTransInfo() { return m_vecTransInformation; }

    CLONE_DISABLE(CMeshData);

public:
    CMeshData();
    virtual ~CMeshData();
};