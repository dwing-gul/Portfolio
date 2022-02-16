#pragma once
#include "Component.h"

enum class DIR_TYPE
{
    RIGHT,
    UP,
    FRONT,
    END
};

class Transform :
    public Component
{
private :
    Vector3 m_localPos;
    Vector3 m_localScale;
    Vector3 m_localRot;

    Vector3 m_localDir[(UINT)DIR_TYPE::END];
    Vector3 m_worldDir[(UINT)DIR_TYPE::END];

    Matrix  m_localMatrix;
    Matrix  m_worldMatrix;

public :
    Vector3 GetLocalPos() { return m_localPos; }
    Vector3 GetLocalScale() { return m_localScale; }
    Vector3 GetLocalRot() { return m_localRot; }
    const Matrix& GetWorldMatrix() { return m_worldMatrix; }
    Vector3 GetWorldScale();
    Vector3 GetWorldPos() { return m_worldMatrix.Translation(); }

    Vector3 GetLocalDir(DIR_TYPE _type) { return m_localDir[(UINT)_type]; }
    Vector3 GetWorldDir(DIR_TYPE _type) { return m_worldDir[(UINT)_type]; }

    void SetLocalPos(Vector3 _pos) { m_localPos = _pos; }
    void SetLocalScale(Vector3 _scale) { m_localScale = _scale; }
    void SetLocalRot(Vector3 _rot) { m_localRot = _rot; }

    virtual void finalUpdate();
    virtual void UpdateData();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(Transform)

public :
    Transform();
    ~Transform();
};

