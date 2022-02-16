#pragma once
#include <Engine/CScript.h>

class CCamera;

class CLightScript :
    public CScript
{
private :
    float m_lightDist; // �߽ɿ��� �������� �Ÿ�
    float m_ratio;
    bool m_bDay; // ������ ������
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

