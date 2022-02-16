#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CGridScript :
    public CScript
{
private :
    CGameObject* m_toolCam;
    Vec3 m_toolCamPos;
    float m_thickness;
    float m_distance;
    UINT m_maxStep;
    float m_alpha;
    Vec4 m_color;

public :
    virtual void update();

    void SetToolCamera(CGameObject* _toolCam) { m_toolCam = _toolCam; }
    void SetGridColor(Vec3 _color) { m_color = _color; }

private :
    void CalculateGrid();

    CLONE(CGridScript);

public :
    CGridScript();
    virtual ~CGridScript();
};

