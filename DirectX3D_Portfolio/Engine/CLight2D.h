#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CLight2D :
    public CComponent
{
private:
    tLightInfo m_info;
    Ptr<CMesh> m_mesh;
    Ptr<CMaterial> m_material;

public:
    void finalUpdate();

    void SetLightType(LIGHT_TYPE _type) { m_info.lightType = _type; }
    void SetAngle(float _angle) { m_info.angle = (_angle / 360.f) * XM_PI; }
    void SetDiffuseColor(Vec3 _color) { m_info.color.diffuse = _color; }
    void SetRange(float _range);
    void SetDir(Vec4 _dir) { m_info.lightDir = _dir;  m_info.lightDir.Normalize(); }
    void SetLightPos(Vec3 _pos);

    tLightInfo& GetLightInfo() { return m_info; }
    Vec3 GetLightPos() { return m_info.lightPos; }

    void render();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CLight2D);

public:
    CLight2D();
    ~CLight2D();
};
