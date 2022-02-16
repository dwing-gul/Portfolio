#pragma once
#include "Component.h"

class GameObject;

class Camera :
    public Component
{
private :
    PROJECTION_TYPE     m_projectionType;
    float               m_far;
    float               m_FOV;
    float               m_scale;
    Matrix              m_viewMatrix;
    Matrix              m_projectionMatrix;
    UINT                m_layerCheck;
    vector<GameObject*> m_vectorTileMap;
    vector<GameObject*> m_vectorForward;
    vector<GameObject*> m_vectorParticle;
    vector<GameObject*> m_vectorPostEffect;

public :
    void SetProjectionType(PROJECTION_TYPE _type) { m_projectionType = _type; }
    void SetFar(float _far) { m_far = _far; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }
    void SetScale(float _scale) { m_scale = _scale; }
    void SetLayerCheck(int _layerIndex)
    {
        if (m_layerCheck & (_layerIndex << 1))
        {
            m_layerCheck &= ~(_layerIndex << 1);
        }
        else
        {
            m_layerCheck |= (_layerIndex << 1);
        }
    }
    void SetLayerCheckAll() { m_layerCheck = 0xffffffff; }

    PROJECTION_TYPE GetProjectionType() { return m_projectionType; }
    float GetFar() { return m_far; }
    float GetFOV() { return m_FOV; }
    float GetScale() { return m_scale; }
    const Matrix& GetViewMatrix() { return m_viewMatrix; }
    const Matrix& GetProjectionMatrix() { return m_projectionMatrix; }

    virtual void finalUpdate();
    void render_tilemap();
    void render_forward();
    void render_particle();
    void render_posteffect();

    void SortObject();

protected :
    void CalViewMatrix();
    void CalProjectionMatrix();

public :
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(Camera);

public:
    Camera();
    ~Camera();
};

