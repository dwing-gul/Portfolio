#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class MeshRender :
    public Component
{
private :
    Ptr<Mesh>     m_mesh;
    Ptr<Material> m_material;

public :
    void render();

    void SetMesh(Ptr<Mesh> _mesh) { m_mesh = _mesh; }
    void SetMaterial(Ptr<Material> _material) { m_material = _material; }

    Ptr<Mesh> GetMesh() { return m_mesh; }
    Ptr<Material> GetCloneMaterial();
    Ptr<Material> GetSharedMaterial() { return m_material; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(MeshRender)

public :
    MeshRender();
    ~MeshRender();
};

