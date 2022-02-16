#pragma once
#include "RigidBody.h"

class HitBox;
class Projectile;
class Item;

struct tSkilCool
{
    float m_skilCoolTime[2]; // ��ų ��Ÿ���� �ֱ� ���� �迭
    bool m_bSkilUse[2]; // ��ų�� ����ߴ��� Ȯ��
};

class Player :
    public RigidBody
{
private :
    static int g_maxHP;
    static int g_curHP;
    static int g_physicalDamage; // ���� ��Ÿ ������
    static int g_magicalDamage; // ���� ��Ÿ ������
    static float g_shiftCoolTime;
    static SKUL g_curSkul;

    GRADE m_grade;
    ATTACK_TYPE m_attackType; // ���õ��� ��Ÿ ���� Ÿ�� ����
    float m_speed; // �̵� �ӵ�
    float m_speedRate; // �̵� �ӵ� ����
    float m_attackSpeed; // ���ݼӵ� ����
    float m_shiftCoolTime;
    float m_dashCoolTime; // �뽬 ��Ÿ��
    float m_damageReduction; // ������ ȿ�� ������ ���� ������ ���� ���
    float m_hpRatio; // �ִ� ü�°� ���� ü���� ����
    float m_criticalRate; // �⺻ ġ��Ÿ Ȯ��
    float m_criticalRatio; // �߰� ġ��Ÿ Ȯ��(������ ȿ�� ������ ���� ġ��Ÿ Ȯ���� ���)
    float m_passiveSkilTime; // ����Ŀ ������ �нú� ��ų�� ���� �ð� üũ
    float m_duration; // ��ų ���� �ð�

    bool m_bGround;
    bool m_bLeftWall;
    bool m_bRightWall;
    bool m_bDash; // �뽬�� �ϰ� �ִ� �������� Ȯ���ϴ� ���� 
    bool m_bKeepDash; // ���� �뽬 �������� Ȯ���ϴ� ����
    bool m_bDie; // �׾����� Ȯ���ϴ� ����
    bool m_bCritical; // ũ��Ƽ���� ������ Ȯ��
    bool m_bJump;
    bool m_bAttack;
    bool m_bHit;
    bool m_bAdapt;
    bool m_bSkilAdapt;
    bool m_bFinish; // ��ų ���� �ð��� �����°�?
    bool m_bFirst; // �� ó�� ������Ʈ �ÿ� ������ٵ� ����Ǿ� �� ������ ���� ���� ����

    int m_jumpCount;
    int m_maxDashCount;
    int m_curDashCount;
    int m_dir; // ���� ĳ���Ͱ� �ٶ󺸰� �ִ� ���� 1 : ������, -1 : ����
    int m_groundCount; // ���� ĳ���Ͱ� ��� �ִ� ���� ����

    wstring m_curAnimName;

    HitBox* m_hitBox;
    Projectile* m_projectile;
    vector<Item*> m_vectorItem; // �������� �����ϱ� ���� �迭 �ִ� 9������ ����ǰ� �ؾߵ�
    vector<SKUL> m_vectorSkul;
    Item* m_essense;

    bool m_bHeadless; // �Ӹ��� �ִ��� Ȯ���ϴ� ����

    vector<tSkilCool> m_vectorSkilCool;

public :
    int GetMaxHP() { return g_maxHP; }
    int GetCurHP() { return g_curHP; }
    int GetPhysicalAtk() { return g_physicalDamage; }
    int GetMagicalAtk() { return g_magicalDamage; }
    int GetDir() { return m_dir; }
    float GetSpeedRate() { return m_speedRate; }
    float GetAttackSpeed() { return m_attackSpeed; }
    float GetHPRatio() { return m_hpRatio; }
    SKUL GetCurSkul() { return g_curSkul; }
    ATTACK_TYPE GetAttackType() { return m_attackType; }
    Item* GetEssense() { return m_essense; }
    vector<Item*>& GetItems() { return m_vectorItem; }
    vector<SKUL>& GetSkuls() { return m_vectorSkul; }
    int GetSkulCount() { return (int)m_vectorSkul.size(); }

    bool IsGround() { return m_bGround; }
    bool IsLeftWall() { return m_bLeftWall; }
    bool IsRightWall() { return m_bRightWall; }
    bool IsJump() { return m_bJump; }

    void SetPhysicalAtk(int _atk) { g_physicalDamage = _atk; }
    void SetMagicalAtk(int _atk) { g_magicalDamage = _atk; }
    void SetMaxHP(int _hp) { g_maxHP = _hp; }
    void SetDashCount(int _dash) { m_maxDashCount = _dash; }
    void SetDamageReduction(float _ratio) { m_damageReduction = _ratio; }
    void SetAttackSpeed(float _ratio) { m_attackSpeed = _ratio;}
    void SetSpeedRate(float _speed) { m_speedRate = _speed; }
    void SetCriticalRatio(float _ratio) { m_criticalRatio = _ratio; }
    void SetDir(int _dir) { m_dir = _dir; }
    void SetAttackType(ATTACK_TYPE _type) { m_attackType = _type; }
    void SetGround(bool _ground) { m_bGround = _ground; }
    void SetGroundCount(int _groundCount) { m_groundCount = _groundCount; }
    void SetFirst(bool _first) { m_bFirst = _first; }
    void Hit(int _damage);

    void CreateHitBox(Vector2 _offset, Vector2 _effectOffset, Vector2 _scale, int _damage);
    void CreateProjectile(Vector2 _offset, Vector2 _scale, const wstring& _relativePath, PROJECTILE_TYPE _type, ATTACK_TYPE _attackType, int _damage);
    void AddItem(Item* _item);
    void AddSkul(SKUL _skul);
    void AddEssense(Item* _essense);
    void SkulInit();
    void DashCountInit() { m_curDashCount = m_maxDashCount; }

    void SetAnimDir(int _dir, wstring _state, bool _repeat);
    void ChangeAnim(wstring _curName, wstring _nextName, int _dir);
    void ChangeSkul();

    virtual void update();
    virtual void lateUpdate();
    virtual void render(HDC _dc);

    virtual void OnColliderEnter(Obj* _other);
    virtual void OnCollider(Obj* _other);
    virtual void OnColliderExit(Obj* _other);

    virtual void Skil();

    CLONE_DISABLE(Player)

private :
    void SetCurHP(int curHP) { g_curHP = curHP; }

public :
    Player();
    virtual ~Player();

    friend class Item;
    friend class Skul_Basic;
};

