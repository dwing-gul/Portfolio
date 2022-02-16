#pragma once
#include <Engine/CScript.h>
#include "define.h"

class CPlayerScript;

class CItemSpawnScript :
    public CScript
{
private :
    ITEM m_arrBox[7]; // ���ڿ� ����ִ� �������� ���� (������ ������ 1����)
    APPEAR m_spawnArea; // ���� ��ġ
    static CGameObject* g_itemBox;
    CGameObject* m_mouse;
    bool m_bAICheck;

public :
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetSpawnArea(APPEAR _spawn) 
    {
        if ((UINT)APPEAR::PLACE_END <= (UINT)_spawn)
            return;

        m_spawnArea = _spawn; 
    }

    void AIGetItems(CPlayerScript* _ai, tBelongings(& _arr)[10]);
    ITEM& GetBoxItem(int _index);
    void LootEnd(int _index);

    void SetCheck(bool _check) { m_bAICheck = _check; }
    bool IsCheck() { 
        int a = 0;
        return m_bAICheck; 
    }

private :
    void FillBox();

public :
    virtual void OnColliderEnter(CCollider3D* _other);
    virtual void OnCollider(CCollider3D* _other);
    virtual void OnColliderExit(CCollider3D* _other);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CItemSpawnScript)

public:
    CItemSpawnScript();
    ~CItemSpawnScript();
};

