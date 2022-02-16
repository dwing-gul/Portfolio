#pragma once
#include <Engine/CScript.h>
#include <Engine/CUIManager.h>
#include "define.h"

class CUIScript :
    public CScript
{
private:
    Vec3 m_originPos;
    Vec3 m_originScale;
    bool m_bChange;
    bool m_bMove; // 드래그 앤 드롭을 통해 움직이는가?

protected :
    CGameObject* m_owner; // 기본적으로 모든 UI의 오너는 플레이어로 설정
    CGameObject* m_target; // 쫓아다니는 UI 등의 다른 대상을 타깃으로 하는 UI를 위해 오너와 별개로 타깃을 설정
    wstring m_targetName;
    UI_TYPE m_uiType;
    bool m_bDrag; // 드래그 앤 드롭을 할 수 있는 ui인가?

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetOwner(CGameObject* _owner) { m_owner = _owner; }
    void SetTarget(CGameObject* _target) { m_target = _target; }
    void SetUIType(UI_TYPE _type) { m_uiType = _type; }
    void SetOriginPos(Vec3 _pos) { m_originPos = _pos; }
    void SetOriginScale(Vec3 _scale) { m_originScale = _scale; }
    void SetDragNDrop(bool _drag) { m_bDrag = _drag; }

    CGameObject* GetOwner() { return m_owner; }
    CGameObject* GetTarget() { return m_target; }
    Vec3 GetOriginPos() { return m_originPos; }
    Vec3 GetOriginScale() { return m_originScale; }
    bool IsMove() { return m_bMove; }
    UI_TYPE GetUIType() { return m_uiType; }

    virtual void PlayerChange(); // 플레이어 전환시 사용할 함수

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    virtual CUIScript* Clone() = 0;

public:
    CUIScript(UINT _type);
    CUIScript(const CUIScript& _origin);
    ~CUIScript();
};

