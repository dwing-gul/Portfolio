#pragma once
#include <Engine\CScript.h>

class CIGItemObjScript :
    public CScript
{
private:
    bool                m_bEquipItem;
    bool                m_bAppearMotion;
    bool                m_bCraftMotion;
    int                 m_curFrame;
    int                 m_objOffFrame;             //�ش� �������϶� ������Ʈ off �ϰ� �ϴ� ����(��ġ ����̺��� ���)
    int                 m_boneIndex;

    CGameObject*        m_ownerObj;
    Ptr<CMaterial>      m_sharedMaterial;
    Ptr<CMaterial>      m_cloneMaterial;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetObjOffFrame(int _frame) { m_objOffFrame = _frame; }
    void SetEquipOption(bool _bOption , int _EquipBoneID);
    void SetAppearMotion(bool _bMotion , float _speed = 1.f);
    void SetOwnerObj(CGameObject* _owner) { m_ownerObj = _owner; }
    void SetCurFramePos();

    void SetBoneIndex(int _boneID) { m_boneIndex = _boneID; }
    CMaterial* GetCurMaterial() { return m_cloneMaterial.Get(); }

    void SetLocalMatrixUpdate(bool _b);
    //imgui��

public:
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CIGItemObjScript)

public:
    CIGItemObjScript();
    CIGItemObjScript(const CIGItemObjScript& _origin);
    ~CIGItemObjScript();
};