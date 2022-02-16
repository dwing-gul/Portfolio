#pragma once
#include "RigidBody.h"

class HitBox;
class Projectile;
class Item;

struct tSkilCool
{
    float m_skilCoolTime[2]; // 스킬 쿨타임을 주기 위한 배열
    bool m_bSkilUse[2]; // 스킬을 사용했는지 확인
};

class Player :
    public RigidBody
{
private :
    static int g_maxHP;
    static int g_curHP;
    static int g_physicalDamage; // 물리 평타 데미지
    static int g_magicalDamage; // 마법 평타 데미지
    static float g_shiftCoolTime;
    static SKUL g_curSkul;

    GRADE m_grade;
    ATTACK_TYPE m_attackType; // 스컬들의 평타 공격 타입 구분
    float m_speed; // 이동 속도
    float m_speedRate; // 이동 속도 비율
    float m_attackSpeed; // 공격속도 비율
    float m_shiftCoolTime;
    float m_dashCoolTime; // 대쉬 쿨타임
    float m_damageReduction; // 아이템 효과 등으로 인한 데미지 감소 계산
    float m_hpRatio; // 최대 체력과 현재 체력의 비율
    float m_criticalRate; // 기본 치명타 확률
    float m_criticalRatio; // 추가 치명타 확률(아이템 효과 등으로 인해 치명타 확률의 계산)
    float m_passiveSkilTime; // 버서커 스컬의 패시브 스킬을 위한 시간 체크
    float m_duration; // 스킬 지속 시간

    bool m_bGround;
    bool m_bLeftWall;
    bool m_bRightWall;
    bool m_bDash; // 대쉬를 하고 있는 상태인지 확인하는 변수 
    bool m_bKeepDash; // 아직 대쉬 상태인지 확인하는 변수
    bool m_bDie; // 죽었는지 확인하는 변수
    bool m_bCritical; // 크리티컬이 떳는지 확인
    bool m_bJump;
    bool m_bAttack;
    bool m_bHit;
    bool m_bAdapt;
    bool m_bSkilAdapt;
    bool m_bFinish; // 스킬 적용 시간이 끝났는가?
    bool m_bFirst; // 맨 처음 업데이트 시에 리지드바디가 적용되어 땅 밑으로 가는 것을 막음

    int m_jumpCount;
    int m_maxDashCount;
    int m_curDashCount;
    int m_dir; // 현재 캐릭터가 바라보고 있는 방향 1 : 오른쪽, -1 : 왼쪽
    int m_groundCount; // 현재 캐릭터가 밟고 있는 땅의 갯수

    wstring m_curAnimName;

    HitBox* m_hitBox;
    Projectile* m_projectile;
    vector<Item*> m_vectorItem; // 아이템을 착용하기 위한 배열 최대 9개까지 적용되게 해야됨
    vector<SKUL> m_vectorSkul;
    Item* m_essense;

    bool m_bHeadless; // 머리가 있는지 확인하는 변수

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

