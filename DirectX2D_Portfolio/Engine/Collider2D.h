#pragma once
#include "Component.h"
#include "Material.h"
#include "Mesh.h"

class Collider2D :
    public Component
{
private :
    Vector2       m_offsetPos;
    Vector2       m_offsetScale;
    Matrix        m_colliderWorldMatrix;
    COLLIDER_TYPE m_type;

    Ptr<Mesh>     m_mesh;
    Ptr<Material> m_material;
    int           m_collisionCount;
    bool          m_bShow;
    bool          m_bTool;
    bool          m_bCollision;
    int           m_hovered;

public :
    Vector2 GetOffsetPos() { return m_offsetPos; }
    Vector2 GetOffsetScale() { return m_offsetScale; }
    Matrix GetWorldMatrix() { return m_colliderWorldMatrix; }
    COLLIDER_TYPE GetColliderType() { return m_type; }
    bool IsCollision() { return m_bCollision; }

    void SetOffsetPos(Vector2 _pos) { m_offsetPos = _pos; }
    void SetOffsetScale(Vector2 _scale) { m_offsetScale = _scale; }
    void SetColliderType(COLLIDER_TYPE _type) { m_type = _type; }

    void OnColliderEnter(Collider2D* _other);
    void OnCollider(Collider2D* _other);
    void OnColliderExit(Collider2D* _other);

    virtual void finalUpdate();
    void render();

    virtual void UpdateData();
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    bool IsObjectHovered();

    CLONE(Collider2D)

public :
    Collider2D();
    Collider2D(const Collider2D& _origin);
    ~Collider2D();
};

