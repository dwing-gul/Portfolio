#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CMeshRender :
    public CComponent
{
private:
    Ptr<CMesh> m_mesh;
    vector<Ptr<CMaterial>> m_vecMaterial;

public:
    void render();
    void render(UINT _materialIndex);
    void render_shadowdepth(UINT _materialIndex);

    void SetMesh(Ptr<CMesh> _mesh);
    void SetMaterial(Ptr<CMaterial> _material, UINT _index = 0);

    Ptr<CMesh> GetMesh() { return m_mesh; }
    Ptr<CMaterial> GetCloneMaterial(UINT _materialIndex = 0);
    Ptr<CMaterial> GetSharedMaterial(UINT _materialIndex = 0);

    size_t GetMaterialCount() { return m_vecMaterial.size(); }
    ULONG64 GetInstID(UINT _materialIndex);
    ULONG64 GetInstShadowID(UINT _materialIndex);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CMeshRender);

public:
    CMeshRender();
    ~CMeshRender();
};


