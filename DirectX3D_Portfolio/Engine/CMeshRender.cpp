#include "pch.h"
#include "CMeshRender.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCollider2D.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"

CMeshRender::CMeshRender() :
	CComponent(COMPONENT_TYPE::MESH_RENDER),
	m_mesh(nullptr)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::render()
{
	if (nullptr == m_mesh || nullptr == m_vecMaterial[0])
		return;

	Transform()->UpdateData();

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}
	else
	{
		CAnimator2D::Clear();
	}

	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (size_t i = 0; i < m_vecMaterial.size(); ++i)
		{
			if (nullptr == m_vecMaterial[i])
				continue;

			m_vecMaterial[i]->SetAnim3D(true);
			m_vecMaterial[i]->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		m_vecMaterial[i]->UpdateData();

		m_mesh->UpdateData((UINT)i);
		m_mesh->render((UINT)i);

		m_vecMaterial[i]->Clear();
	}

	if (Animator3D())
	{
		Animator3D()->ClearData();
	}
}

void CMeshRender::render(UINT _materialIndex)
{
	if (nullptr == m_mesh || nullptr == m_vecMaterial[0])
		return;

	Transform()->UpdateData();

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}
	else
	{
		CAnimator2D::Clear();
	}

	if (Animator3D())
	{
		Animator3D()->UpdateData();
		m_vecMaterial[_materialIndex]->SetAnim3D(true);
		m_vecMaterial[_materialIndex]->SetBoneCount(Animator3D()->GetBoneCount());
	}

	m_vecMaterial[_materialIndex]->UpdateData();

	m_mesh->UpdateData(_materialIndex);
	m_mesh->render(_materialIndex);

	m_vecMaterial[_materialIndex]->Clear();

	if (Animator3D())
	{
		Animator3D()->ClearData();
	}
}

void CMeshRender::render_shadowdepth(UINT _materialIndex)
{
	if (nullptr == m_mesh || nullptr == m_vecMaterial[_materialIndex])
		return;

	Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(L"ShadowDepthMaterial");

	// Animation 확인
	if (Animator3D())
	{
		// 재질에 3D Animation 이 있음을 알림
		material->SetAnim3D(true);
		material->SetBoneCount(Animator3D()->GetBoneCount());

		// 최종행렬 연산
		Animator3D()->UpdateData();
	}

	material->UpdateData();
	m_mesh->UpdateData(_materialIndex);
	m_mesh->render(_materialIndex);
}

void CMeshRender::SetMesh(Ptr<CMesh> _mesh)
{
	m_mesh = _mesh;

	int resizeCount = 1;

	if (nullptr != m_mesh)
		resizeCount = m_mesh->GetSubsetCount();

	vector<Ptr<CMaterial>> vector;
	vector.resize(resizeCount);
	m_vecMaterial.swap(vector);
}

void CMeshRender::SetMaterial(Ptr<CMaterial> _material, UINT _index)
{
	if (m_vecMaterial.size() <= _index)
	{
		m_vecMaterial.push_back(_material);
		return;
	}

	m_vecMaterial[_index] = _material;
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial(UINT _materialIndex)
{
	if (m_vecMaterial.size() <= _materialIndex || nullptr == m_vecMaterial[_materialIndex])
		return nullptr;

	m_vecMaterial[_materialIndex] = m_vecMaterial[_materialIndex]->Clone();
	return m_vecMaterial[_materialIndex];
}

Ptr<CMaterial> CMeshRender::GetSharedMaterial(UINT _materialIndex)
{
	if (m_vecMaterial.size() <= _materialIndex || nullptr == m_vecMaterial[_materialIndex])
		return nullptr;

	return m_vecMaterial[_materialIndex];
}

ULONG64 CMeshRender::GetInstID(UINT _materialIndex)
{
	if (m_mesh == NULL || m_vecMaterial[_materialIndex] == NULL)
		return 0;

	uInstID id{ (UINT)m_mesh->GetID(), (WORD)m_vecMaterial[_materialIndex]->GetID(), (WORD)_materialIndex };

	return id.ID;
}

ULONG64 CMeshRender::GetInstShadowID(UINT _materialIndex)
{
	if (m_mesh == NULL || m_vecMaterial[_materialIndex] == NULL)
		return 0;

	Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(L"ShadowDepthMaterial");

	uInstID id{ (UINT)m_mesh->GetID(), (WORD)material->GetID(), (WORD)_materialIndex };
	return id.ID;
}

void CMeshRender::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	SaveResourceToFile(m_mesh, _file);
	size_t materialCount = m_vecMaterial.size();
	SaveToFile(&materialCount, _file);

	for (size_t i = 0; i < materialCount; ++i)
	{
		SaveResourceToFile(m_vecMaterial[i], _file);
	}
}

void CMeshRender::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	LoadResourceFromFile(m_mesh, _file);

	size_t materialCount = 0;
	LoadFromFile(&materialCount, _file);

	for (size_t i = 0; i < materialCount; ++i)
	{
		Ptr<CMaterial> material;
		LoadResourceFromFile(material, _file);
		m_vecMaterial.push_back(material);
	}
}