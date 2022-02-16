#pragma once
#include "CComponent.h"

enum class DIR_TYPE
{
    RIGHT,
    UP,
    FRONT,
    END
};

class CTransform :
    public CComponent
{
private:
    Vec3        m_localPos;
    Vec3        m_localScale;
    Vec3        m_localRot;
    Matrix      m_axisRot;

    Vec3        m_localDir[(UINT)DIR_TYPE::END];
    Vec3        m_worldDir[(UINT)DIR_TYPE::END];

    Matrix      m_localMatrix;
    Matrix      m_worldMatrix;
    Matrix      m_newWorldMatrix;
    Matrix      m_worldMatrixInverse;

    float       m_boundingRange;
    bool        m_bFrustumCheck;

    bool        m_bLocalUpdateData;
    bool        m_bSetWorld;

public:
    Vec3 GetLocalPos() { return m_localPos; }
    Vec3 GetLocalScale() { return m_localScale; }
    Vec3 GetLocalRot() { return m_localRot; }
    const Matrix& GetWorldMatrix() { return m_worldMatrix; }
    const Matrix& GetWorldMatrixInverse() { return m_worldMatrixInverse; }
    Vec3 GetWorldScale();
    Vec3 GetWorldPos() { return m_worldMatrix.Translation(); }
    Vec3 GetLocalDir(DIR_TYPE _type) { return m_localDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) { return m_worldDir[(UINT)_type]; }
    
    Vec3 GetWorldPos_XZ() { return m_worldMatrix.Translation().XZ(); }

    void SetLocalPos(Vec3 _pos) { m_localPos = _pos; }
    void SetLocalPos_Y(float _y) { m_localPos.y = _y; }
    void SetLocalScale(Vec3 _scale) { m_localScale = _scale; }
    void SetLocalRot(Vec3 _rot) { m_localRot = _rot; }
    void SetLocalRot_Y(float _rot) { m_localRot.y = _rot; }
    void SetLocalRot_Euler(Vec3 _rot) { m_localRot = (_rot * XM_PI) / 180.f; }
    void SetWorldMatrix(Matrix _world) { m_newWorldMatrix = _world; m_bSetWorld = true; }
    void SetWorld(bool _true) { m_bSetWorld = _true; }

    void SetBoundingRange(float _range) { m_boundingRange = _range; }
    float GetBoundingRange() { return m_boundingRange; }

    void UseFrustumCheck(bool _check) { m_bFrustumCheck = _check; }
    bool IsFrustumCheck() { return m_bFrustumCheck; }
    void SetLocalMatrixUpdate(bool _b) { m_bLocalUpdateData = _b; }

    virtual void finalUpdate();
    virtual void UpdateData();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CTransform);

public:
    CTransform();
    ~CTransform();
};
