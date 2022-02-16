#pragma once
#include <Engine\CScript.h>
#include <Engine/CNaviTileNode.h>
#include "define.h"
#include "CScriptManager.h"

class CObjectScript :
    public CScript
{
protected :
    static CGameObject* g_itemBox;

protected:
    CNaviTileNode*          m_curNode;
    CGameObject*            m_naviMesh;
    CGameObject*            m_trackingUI;
    
    CPrefab*                m_hitEffect;

    int                     m_level;
    int                     m_maxLevel;
    float                   m_maxHP; // �ִ� ü��
    float                   m_curHP; // ���� ü��

    float                   m_maxStamina; // �ִ� ���׹̳�
    float                   m_curStamina; // ���� ���׹̳�

    float                   m_offense; // ���ݷ�
    float                   m_defense; // ����
    float                   m_defense_Buff;
    float                   m_moveSpeed; // �̵� �ӵ�
    float                   m_attackSpeed; // ���� �ӵ�
    float                   m_healthRegen; // ü�� ���
    float                   m_staminaRegen; // ���׹̳� ���
    float                   m_criticalChance; // ġ��Ÿ��
    float                   m_criticalDamage; // ġ��Ÿ ������ ����
    float                   m_FOV; // ���� �þ� ����
    float                   m_attackRange; // ���� ����
    float                   m_rotTime; // ȸ���� ������ �ð�
    float                   m_rotAngle; // ȸ�� ����

    float                   m_stunTime; // ���� �ð�
    float                   m_slowTime; // ��ȭ �ð�
    float                   m_silenceTime; // ħ�� �ð�
    float                   m_defReduceTime; // �氨 �ð�
    float                   m_defReduce; // ���� ����
    float                   m_defensePenetration; // �޴� ��� ����
    float                   m_slowRate; // �̼� ���� ����
    float                   m_nonMoveTime;

    bool                    m_bStun; // ����
    bool                    m_bSlow; // ��ȭ
    bool                    m_bSilence; //ħ��
    bool                    m_bNonStop; // ���� �Ұ�
    bool                    m_bDefReduce; // ��� ����
    bool                    m_bNonMove; // �ൿ �Ұ�

    float                   m_visionRange; // ������Ʈ ������Ʈ�� ���� �þ� ����

    bool                    m_bAttack; // ���� ���� Ȯ��
    bool                    m_bRot;
    bool                    m_bFindPath; // ��ã�⸦ ���� bool ��
    bool                    m_bCharacterDead; // ���� ������Ʈ�� ����ʹ� ������ ĳ���Ͱ� ���� �������� Ȯ���ϴ� ����
    bool                    m_bHit;
    bool                    m_bBuff;
    bool                    m_bMotion;

    bool                    m_bAttackMotion;            //���� ���������
    bool                    m_bFristAttackMotion;
    bool                    m_bAICheck;

    CGameObject*            m_attackBox; // ��ų ���� ������ �浹ü�� ����� ������Ʈ
    CGameObject*            m_attackTarget; // �⺻ ���� �� ���� ���
    CGameObject*            m_mouse;
    list<Vec3>              m_waypoints; // astar �̿�� ��θ� �����ϱ� ���� ����Ʈ

    Ptr<CSound>             m_hitSound;
    Ptr<CSound>             m_attackSound;
    Ptr<CSound>             m_dieSound;

    bool IsPlaySound();

private:
    virtual void CharacterDead() {};
   
protected:
    virtual void StatsReSet() {};

public:
    float GetCurHP() { return m_curHP; }
    float GetCurSP() { return m_curStamina; }
    int GetLevel() { return m_level; }

    void SetDefense_Buf(float _defense) { m_defense_Buff = _defense; }
    bool IsHit() { return m_bHit; }
    bool IsCharacterDead() { return m_bCharacterDead; }
    bool IsAttack() { return m_bAttack; }
    bool IsMotion() { return m_bMotion; }
    float GetAttackRange() { return m_attackRange; }
    float GetFOV() { return m_FOV; }

    void SetStun(bool _stun, float _time) { m_bStun = _stun; m_stunTime = _time; }
    void SetSlow(bool _slow, float _time, float _slowRate) { m_bSlow = _slow; m_slowTime = _time; m_slowRate = _slowRate; }
    void SetDefReduce(bool _reduce, float _time, float _reduceRate) { m_bDefReduce = _reduce; m_defReduceTime = _time; m_defReduce = _reduceRate; }
    void SetSilence(bool _silence, float _time) { m_bSilence = _silence; m_silenceTime = _time; }
    void SetDefPenetration(float _penetration) { m_defensePenetration = _penetration; }
    void SetNonStop(bool _nonstop) { m_bNonStop = _nonstop; }
    void SetNonMove(bool _nonmove, float _time) { m_bNonMove = _nonmove; m_nonMoveTime = _time; }

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate() {}

    virtual void Attack() {}
    virtual void Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner = nullptr);

    void SetAttackTarget(CGameObject* _target) { m_attackTarget = _target; }

    bool IsCheck() { return m_bAICheck; }
    void SetCheck(bool _check) { m_bAICheck = _check; }

    virtual void OnColliderEnter(CCollider3D* _other) {}
    virtual void OnCollider(CCollider3D* _other) {}
    virtual void OnColliderExit(CCollider3D* _other) {}

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CObjectScript)

public:
    CObjectScript(UINT _type = (UINT)SCRIPT_TYPE::OBJECTSCRIPT);
    CObjectScript(const CObjectScript& _origin);
    ~CObjectScript();   
};

