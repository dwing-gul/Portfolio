#pragma once
#include "CComponent.h"
#include "CFrustum.h"

class CGameObject;

class CCamera :
    public CComponent
{
protected:
    CFrustum m_frustum;
    tRay m_ray;
    PROJECTION_TYPE m_projectionType;
    float m_far;
    float m_FOV;
    float m_scale;
    CAMERA_TYPE m_cameraType;
    bool m_bColliderShow;

    Matrix m_viewMatrix;
    Matrix m_projectionMatrix;

    Matrix m_viewMatrixInverse;
    Matrix m_projectionMatrixInverse;

    UINT m_layerCheck;

    map<ULONG64, vector<tInstObj>>  m_mapInstGroup_D;
    map<ULONG64, vector<tInstObj>>  m_mapInstGroup_F;
    map<INT_PTR, vector<tInstObj>>  m_mapSingleObj;

    vector<CGameObject*>            m_vecParticle;
    vector<CGameObject*>            m_vecPostEffect;

    Vec3                            m_prevPos;
    Vec3                            m_prevRot;

    vector<CGameObject*>            m_vecCollider3D;
    vector<CGameObject*>            m_vecCollider2D;

public:
    void SetProjectionType(PROJECTION_TYPE _type) { m_projectionType = _type; }
    void SetCameraType(CAMERA_TYPE _type) { m_cameraType = _type; }
    void SetFar(float _far) { m_far = _far; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }
    void SetScale(float _scale) { m_scale = _scale; }
    void SetLayerCheck(int _layerIndex)
    {
        if (m_layerCheck & (1 << _layerIndex))
        {
            m_layerCheck &= ~(1 << _layerIndex);
        }
        else
        {
            m_layerCheck |= (1 << _layerIndex);
        }
    }
    void SetLayerCheckAll() { m_layerCheck = 0xffffffff; }
    void SetColliderShow(bool _bShow) { m_bColliderShow = _bShow; }

    PROJECTION_TYPE GetProjectionType() { return m_projectionType; }
    CAMERA_TYPE GetCameraType() { return m_cameraType; }
    float GetFar() { return m_far; }
    float GetFOV() { return m_FOV; }
    float GetScale() { return m_scale; }
    const Matrix& GetViewMatrix() { return m_viewMatrix; }
    const Matrix& GetProjectionMatrix() { return m_projectionMatrix; }
    const Matrix& GetViewInverseMatrix() { return m_viewMatrixInverse; }
    const Matrix& GetProjectionInverseMatrix() { return m_projectionMatrixInverse; }
    CFrustum& GetFrustum() { return m_frustum; }
    const tRay& GetRay() { return m_ray; }

    virtual void finalUpdate();
    void render_deferred();
    void render_forward();
    void render_particle();
    void render_posteffect();

    void SortObject();

public:
    void SetPrevPos(Vec3 pos) { m_prevPos = pos; }
    void SetPrevRot(Vec3 rot) { m_prevRot = rot; }

    Vec3 GetPrevPos() { return m_prevPos; }
    Vec3 GetPrevRot() { return m_prevRot; }

protected:
    void CalViewMatrix();
    void CalProjectionMatrix();
    void CalRay();

public:
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CCamera);

public:
    CCamera();
    ~CCamera();
};

