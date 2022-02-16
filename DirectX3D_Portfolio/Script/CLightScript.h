#pragma once
#include <Engine/CScript.h>

class CCamera;

class CLightScript :
    public CScript
{
private :
    float m_lightDist; // 중심에서 빛까지의 거리
    float m_ratio;
    bool m_bDay; // 낮인지 밤인지
    CCamera* m_cam;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    void SetRatio(float _ratio) { m_ratio = _ratio; }
    void SetDist(float _dist) { m_lightDist = _dist; }
    void SetDay(bool _day) { m_bDay = _day; }

private :
    void RotAxis();

    CLONE(CLightScript)

public:
    CLightScript();
    ~CLightScript();
};

