#include "pch.h"
#include "CFrustum.h"
#include "CCamera.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"

CFrustum::CFrustum(CCamera* _cam) :
	m_cam(_cam),
	m_frustumBuffer(nullptr)
{
	// 투영좌표계 기준 점 8개 찍어놓기

	// Near		Far
	// 		/	4 ----- 5
	// 	   /	|		|
	// 			|		|
	//0 -- 1	7 ----- 6
	//|    |   /
	//3 -- 2  /
	m_arrProjection[0] = Vec3(-1.f, 1.f, 0.f);
	m_arrProjection[1] = Vec3(1.f, 1.f, 0.f);
	m_arrProjection[2] = Vec3(1.f, -1.f, 0.f);
	m_arrProjection[3] = Vec3(-1.f, -1.f, 0.f);

	m_arrProjection[4] = Vec3(-1.f, 1.f, 1.f);
	m_arrProjection[5] = Vec3(1.f, 1.f, 1.f);
	m_arrProjection[6] = Vec3(1.f, -1.f, 1.f);
	m_arrProjection[7] = Vec3(-1.f, -1.f, 1.f);

	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"FrustumMaterial");
	m_frustumBuffer = new CStructuredBuffer;
}

CFrustum::~CFrustum()
{
	Safe_Delete_Ptr(m_frustumBuffer);
}

void CFrustum::finalUpdate()
{
	Matrix viewInverseMatrix = m_cam->GetViewInverseMatrix();
	Matrix projectionInverseMatrix = m_cam->GetProjectionInverseMatrix();
	Matrix inverseMatrix = projectionInverseMatrix * viewInverseMatrix;

	Vec3 worldPos[8] = {};

	for (int i = 0; i < 8; ++i)
	{
		worldPos[i] = XMVector3TransformCoord(m_arrProjection[i], inverseMatrix);
	}

	// 월드 스페이스 평면 6개를 구한다.	
	m_arrFace[(UINT)FACE_TYPE::FT_NEAR] = XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]);
	m_arrFace[(UINT)FACE_TYPE::FT_FAR] = XMPlaneFromPoints(worldPos[5], worldPos[4], worldPos[7]);
	m_arrFace[(UINT)FACE_TYPE::FT_UP] = XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]);
	m_arrFace[(UINT)FACE_TYPE::FT_DOWN] = XMPlaneFromPoints(worldPos[6], worldPos[7], worldPos[2]);
	m_arrFace[(UINT)FACE_TYPE::FT_LEFT] = XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]);
	m_arrFace[(UINT)FACE_TYPE::FT_RIGHT] = XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]);

	m_frustumBuffer->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(Vec3), 8, worldPos);
}

void CFrustum::render()
{
	if (nullptr == m_mesh || nullptr == m_material)
		return;

	m_cam->Transform()->UpdateData();

	m_mesh->UpdateData(0);
	m_material->UpdateData();
	m_frustumBuffer->UpdateData(13);

	m_mesh->render(0);

	m_material->Clear();

	m_frustumBuffer->Clear();
}

bool CFrustum::CheckFrustum(const Vec3& _pos)
{
	for (int i = 0; i < (UINT)FACE_TYPE::FT_END; ++i)
	{
		Vec3 normal = m_arrFace[i];

		if (normal.Dot(_pos) + m_arrFace[i].w > 0)
			return false;
	}

	return true;
}

bool CFrustum::CheckFrustumSphere(const Vec3& _pos, float _radius)
{
	for (int i = 0; i < (UINT)FACE_TYPE::FT_END; ++i)
	{
		Vec3 normal = m_arrFace[i];

		if (normal.Dot(_pos) + m_arrFace[i].w > _radius)
			return false;
	}

	return true;
}