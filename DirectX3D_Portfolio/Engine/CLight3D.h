#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CLight3D :
    public CComponent
{
private:
    tLightInfo m_info;
    Ptr<CMesh> m_mesh;
    Ptr<CMaterial> m_material;
    CGameObject* m_camObj;

    UINT m_lightIndex;

public:
    void finalUpdate();
    void render();
    void render_shadowdepth();

    void SetLightType(LIGHT_TYPE _type);
    void SetAngle(float _angle) { m_info.angle = (_angle / 360.f) * XM_PI; }
    void SetDiffuseColor(Vec3 _color) { m_info.color.diffuse = _color; }
    void SetSpecularColor(Vec3 _color) { m_info.color.specular = _color; }
    void SetAmbientColor(Vec3 _color) { m_info.color.ambient = _color; }
    void SetRange(float _range);

    tLightInfo& GetLightInfo() { return m_info; }
    Vec3 GetLightPos() { return m_info.lightPos; }
    CGameObject* GetLightCam() { return m_camObj; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CLight3D);

public:
    CLight3D();
    ~CLight3D();
};

