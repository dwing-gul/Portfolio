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
    int                 m_objOffFrame;             //해당 프레임일때 오브젝트 off 하게 하는 역할(망치 드라이브의 경우)
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
    //imgui용

public:
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CIGItemObjScript)

public:
    CIGItemObjScript();
    CIGItemObjScript(const CIGItemObjScript& _origin);
    ~CIGItemObjScript();
};