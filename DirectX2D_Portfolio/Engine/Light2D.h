#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class Light2D :
    public Component
{
private :
    tLightInfo    m_info;
    Ptr<Mesh>     m_mesh;
    Ptr<Material> m_material;
    bool          m_bShowBoundary;
    bool          m_bTool;

public :
    virtual void finalUpdate();
    void render();

    void SetLightType(LIGHT_TYPE _type) { m_info.lightType = _type; }
    void SetAngle(float _angle) { m_info.angle = (_angle / 360.f) * XM_PI; }
    void SetDiffuseColor(Vector3 _color) { m_info.color.diffuse = _color; }
    void SetAxis(Vector3 _axis) { m_info.axis = _axis.Normalize(); }
    void SetLightPos(Vector3 _pos);
    void SetRange(float _range);

    tLightInfo& GetLightInfo() { return m_info; }
    Vector3 GetLightPos() { return m_info.lightPos; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(Light2D)

public :
    Light2D();
    ~Light2D();
};

