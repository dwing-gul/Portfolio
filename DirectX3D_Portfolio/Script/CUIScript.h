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
    bool m_bMove; // �巡�� �� ����� ���� �����̴°�?

protected :
    CGameObject* m_owner; // �⺻������ ��� UI�� ���ʴ� �÷��̾�� ����
    CGameObject* m_target; // �Ѿƴٴϴ� UI ���� �ٸ� ����� Ÿ������ �ϴ� UI�� ���� ���ʿ� ������ Ÿ���� ����
    wstring m_targetName;
    UI_TYPE m_uiType;
    bool m_bDrag; // �巡�� �� ����� �� �� �ִ� ui�ΰ�?

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

    virtual void PlayerChange(); // �÷��̾� ��ȯ�� ����� �Լ�

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    virtual CUIScript* Clone() = 0;

public:
    CUIScript(UINT _type);
    CUIScript(const CUIScript& _origin);
    ~CUIScript();
};

