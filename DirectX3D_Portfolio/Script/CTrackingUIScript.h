#pragma once
#include "CUIScript.h"

enum class TRACKING_TARGET_TYPE
{
    MONSTER,
    PLAYER,
    ITEM_SPAWN,
    END
};

class CTrackingUIScript :
    public CUIScript
{
private :
    static float g_zPos;

private :
    Vec3 m_offsetPos;
    TRACKING_TARGET_TYPE m_trackingType; // 야생동물인지 캐릭터인지 아이템 스폰인지 확인하는 변수
    CGameObject* m_cam;
    Vec3 m_trackingScale;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetOffsetPos(Vec3 _offset) { m_offsetPos = _offset; }
    void SetTrackingType(TRACKING_TARGET_TYPE _type) { m_trackingType = _type; }

    TRACKING_TARGET_TYPE GetTrackingType() { return m_trackingType; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CTrackingUIScript)

public:
    CTrackingUIScript();
    CTrackingUIScript(const CTrackingUIScript& _origin);
    ~CTrackingUIScript();
};

