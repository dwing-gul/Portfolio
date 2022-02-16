#pragma once
#include "CEntity.h"
#include "CMesh.h"
#include "CMaterial.h"

enum class FACE_TYPE
{
    FT_NEAR,
    FT_FAR,
    FT_UP,
    FT_DOWN,
    FT_RIGHT,
    FT_LEFT,
    FT_END
};

class CCamera;
class CStructuredBuffer;

class CFrustum :
    public CEntity
{
    CCamera* m_cam;
    CStructuredBuffer* m_frustumBuffer;

    Ptr<CMesh> m_mesh;
    Ptr<CMaterial> m_material;

    Vec3 m_arrProjection[8];
    Vec4 m_arrFace[(UINT)FACE_TYPE::FT_END];

public:
    void finalUpdate();
    void render();

    bool CheckFrustum(const Vec3& _pos);
    bool CheckFrustumSphere(const Vec3& _pos, float _radius);

    CLONE(CFrustum);

public:
    CFrustum(CCamera* _cam);
    ~CFrustum();
};

