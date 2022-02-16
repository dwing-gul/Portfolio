#include "pch.h"
#include "CLight3D.h"
#include "CTransform.h"
#include "CRenderManager.h"
#include "CLightCamera.h"

CLight3D::CLight3D() :
	CComponent(COMPONENT_TYPE::LIGHT3D),
	m_info{},
	m_lightIndex(-1)
{
	m_camObj = new CGameObject;
	m_camObj->AddComponent(new CTransform);
	m_camObj->AddComponent(new CLightCamera);

	m_camObj->Camera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
	m_camObj->Camera()->SetScale(2.f);
	m_camObj->Camera()->SetLayerCheckAll();
	m_camObj->Camera()->SetLayerCheck(31);
	m_camObj->Camera()->SetLayerCheck(30);

	SetLightType(LIGHT_TYPE::DIR);		//디폴트 값 세팅
}

CLight3D::~CLight3D()
{
	Safe_Delete_Ptr(m_camObj);
}

void CLight3D::finalUpdate()
{
	m_info.lightPos = Transform()->GetWorldPos();
	m_info.lightDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_info.lightDir.x = -m_info.lightDir.x;
	m_info.lightDir.z = -m_info.lightDir.z;
	m_lightIndex = CRenderManager::GetInst()->RegisterLight3D(this);

	m_camObj->Transform()->SetLocalPos(Transform()->GetLocalPos());
	m_camObj->Transform()->SetLocalScale(Transform()->GetLocalScale());
	m_camObj->Transform()->SetLocalRot(Transform()->GetLocalRot());

	m_camObj->finalUpdate_ex();
}

void CLight3D::render()
{
	Transform()->UpdateData();

	if (LIGHT_TYPE::DIR == m_info.lightType)
	{
		Ptr<CTexture> shadowdepthTex = CResourceManager::GetInst()->FindRes<CTexture>(L"ShadowDepthTargetTex");
		m_material->SetData(SHADER_PARAM::TEXTURE_3, shadowdepthTex.Get());

		Matrix matrixVP = m_camObj->Camera()->GetViewMatrix() * m_camObj->Camera()->GetProjectionMatrix();
		m_material->SetData(SHADER_PARAM::MATRIX_0, &matrixVP);
	}

	m_material->SetData(SHADER_PARAM::INT_0, &m_lightIndex);
	m_material->UpdateData();

	m_mesh->UpdateData(0);
	m_mesh->render(0);

	m_material->Clear();
}

void CLight3D::render_shadowdepth()
{
	((CLightCamera*)m_camObj->Camera())->SortShadowObject();
	((CLightCamera*)m_camObj->Camera())->render_shadowdepth();
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_info.lightType = _type;
	m_material = CRenderManager::GetInst()->GetLightMaterial(m_info.lightType);

	if (LIGHT_TYPE::DIR == m_info.lightType)
	{
		m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	}
	else if (LIGHT_TYPE::POINT == m_info.lightType)
	{
		m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"SphereMesh");
	}
	else
	{
		m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"ConeMesh");
	}
}

void CLight3D::SetRange(float _range)
{
	m_info.range = _range;

	Transform()->SetLocalScale(Vec3(m_info.range, m_info.range, m_info.range));
}

void CLight3D::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	SaveToFile(&m_info, _file);
}

void CLight3D::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	LoadFromFile(&m_info, _file);

	SetLightType(m_info.lightType);
}