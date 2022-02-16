#include "pch.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CTerrain.h"
#include "CAnimator3D.h"
#include "CInstancingBuffer.h"
#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CMRT.h"
#include "CScene.h"
#include "CLayer.h"
#include "CCollider2D.h"
#include "CCollider3D.h"

CCamera::CCamera() :
	CComponent(COMPONENT_TYPE::CAMERA),
	m_frustum(this),
	m_projectionType(PROJECTION_TYPE::PERSPECTIVE),
	m_cameraType(CAMERA_TYPE::MAIN),
	m_far(10000.f),
	m_FOV(XM_PI / 3.f),
	m_scale(0.1f),
	m_layerCheck(0),
	m_bColliderShow(false)
{
	m_viewMatrix = XMMatrixIdentity();
	m_projectionMatrix = XMMatrixIdentity();
}

CCamera::~CCamera()
{
}

void CCamera::finalUpdate()
{
	CalViewMatrix();
	CalProjectionMatrix();
	CalRay();

	m_frustum.finalUpdate();

	CRenderManager::GetInst()->RegisterCamera(this);
}

void CCamera::render_deferred()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	g_transform.viewMatrixInverse = m_viewMatrixInverse;
	g_transform.projectionMatrixInverse = m_projectionMatrixInverse;

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1 || pair.second[0].obj->Animator2D() || //pair.second[0].obj->Animator3D() || 
			pair.second[0].obj->MeshRender()->GetSharedMaterial(pair.second[0].materialIndex)->GetShader()->GetInstVS() == nullptr || pair.second[0].obj->Terrain())
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter = m_mapSingleObj.find((INT_PTR)pair.second[i].obj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].obj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* obj = pair.second[0].obj;
		Ptr<CMesh> mesh = obj->MeshRender()->GetMesh();
		Ptr<CMaterial> material = obj->MeshRender()->GetSharedMaterial(pair.second[0].materialIndex);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int rowIndex = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.worldMatrix = pair.second[i].obj->Transform()->GetWorldMatrix();
			tInstData.matrixWV = tInstData.worldMatrix * m_viewMatrix;
			tInstData.matrixWVP = tInstData.matrixWV * m_projectionMatrix;
			tInstData.matrixVP = m_viewMatrix * m_projectionMatrix;
			tInstData.matrixV = m_viewMatrix;

			if (pair.second[i].obj->Animator3D())
			{
				pair.second[i].obj->Animator3D()->UpdateData();
				tInstData.rowIndex = rowIndex++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMatrix(pair.second[i].obj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.rowIndex = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			material->SetAnim3D(true); // Animation Mesh 알리기
			material->SetBoneCount(mesh->GetBoneCount());
		}

		material->UpdateData_Inst();
		mesh->UpdateData_Inst(pair.second[0].materialIndex);
		mesh->render_instancing(pair.second[0].materialIndex);

		// 정리
		material->Clear();
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].obj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			if (instObj.obj->Terrain())
			{
				instObj.obj->Terrain()->render();
			}
			else
			{
				instObj.obj->MeshRender()->render(instObj.materialIndex);
			}
		}

		if (pair.second[0].obj->Animator3D())
		{
			pair.second[0].obj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_forward()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	g_transform.viewMatrixInverse = m_viewMatrixInverse;
	g_transform.projectionMatrixInverse = m_projectionMatrixInverse;

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};

	map<INT_PTR, vector<tInstObj>> mapFont;

	for (auto& pair : m_mapInstGroup_F)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1 || pair.second[0].obj->Animator2D() || pair.second[0].obj->Animator3D() ||
			pair.second[0].obj->MeshRender()->GetSharedMaterial(pair.second[0].materialIndex)->GetShader()->GetInstVS() == nullptr || 
			pair.second[0].obj->Terrain() || pair.second[0].obj->IsFont())
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter = m_mapSingleObj.find((INT_PTR)pair.second[i].obj);

				if (pair.second[i].obj->IsFont())
				{
					mapFont.insert(make_pair((INT_PTR)pair.second[i].obj, vector<tInstObj>{pair.second[i]}));
				}
				else
				{
					if (iter != m_mapSingleObj.end())
						iter->second.push_back(pair.second[i]);
					else
					{
						m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].obj, vector<tInstObj>{pair.second[i]}));
					}
				}
			}
			continue;
		}

		CGameObject* obj = pair.second[0].obj;
		Ptr<CMesh> mesh = obj->MeshRender()->GetMesh();
		Ptr<CMaterial> material = obj->MeshRender()->GetSharedMaterial(pair.second[0].materialIndex);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int rowIndex = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.worldMatrix = pair.second[i].obj->Transform()->GetWorldMatrix();
			tInstData.matrixWV = tInstData.worldMatrix * m_viewMatrix;
			tInstData.matrixWVP = tInstData.matrixWV * m_projectionMatrix;
			tInstData.matrixVP = m_viewMatrix * m_projectionMatrix;
			tInstData.matrixV = m_viewMatrix;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			material->SetAnim3D(true); // Animation Mesh 알리기
			material->SetBoneCount(mesh->GetBoneCount());
		}

		material->UpdateData_Inst();
		mesh->UpdateData_Inst(pair.second[0].materialIndex);
		mesh->render_instancing(pair.second[0].materialIndex);

		// 정리
		material->Clear();
	}

	for (auto& pair : mapFont)
	{
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			map<INT_PTR, vector<tInstObj>>::iterator iter = m_mapSingleObj.find((INT_PTR)pair.second[i].obj << 1);

			if (iter != m_mapSingleObj.end())
				iter->second.push_back(pair.second[i]);
			else
			{
				m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].obj << 1, vector<tInstObj>{pair.second[i]}));
			}
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].obj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			if (instObj.obj->Terrain())
			{
				instObj.obj->Terrain()->render();
			}
			else if(instObj.obj->MeshRender())
			{
				instObj.obj->MeshRender()->render(instObj.materialIndex);
			}
			
			for (size_t i = 0; i < instObj.obj->GetScripts().size(); ++i)
			{
				instObj.obj->render_script((int)i);
			}
		}

		if (pair.second[0].obj->Animator3D())
		{
			pair.second[0].obj->Animator3D()->ClearData();
		}
	}

	if (m_bColliderShow)
	{
		for (int i = 0; i < m_vecCollider3D.size(); ++i)
		{
			m_vecCollider3D[i]->Collider3D()->render();
		}

		for (int i = 0; i < m_vecCollider2D.size(); ++i)
		{
			m_vecCollider2D[i]->Collider2D()->render();
		}
	}
}

void CCamera::render_particle()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	g_transform.viewMatrixInverse = m_viewMatrixInverse;
	g_transform.projectionMatrixInverse = m_projectionMatrixInverse;

	for (size_t i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->render();
	}
}

void CCamera::render_posteffect()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	g_transform.viewMatrixInverse = m_viewMatrixInverse;
	g_transform.projectionMatrixInverse = m_projectionMatrixInverse;

	for (size_t i = 0; i < m_vecPostEffect.size(); ++i)
	{
		CRenderManager::GetInst()->CopyBackBuffer();
		m_vecPostEffect[i]->render();
	}
}


void CCamera::SortObject()
{
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();

	m_vecParticle.clear();
	m_vecPostEffect.clear();

	m_vecCollider3D.clear();
	m_vecCollider2D.clear();

	CScene* curScene = CSceneManager::GetInst()->GetCurScene();

	if (nullptr == curScene)
		return;

	for (int j = 0; j < MAX_LAYER; ++j)
	{
		/*if (!CSceneManager::GetInst()->GetCurScene()->GetLayer(i)->GetLayerAction())
		{
			continue;
		}*/

		if (m_layerCheck & (1 << j))
		{
			CLayer* layer = curScene->GetLayer(j);
			const vector<CGameObject*>& vectorAllObj = layer->GetAllObj();
			for (size_t i = 0; i < vectorAllObj.size(); ++i)
			{
				CGameObject* obj = vectorAllObj[i];

				if(!obj->GetRenderCheck() || !obj->IsOn())
					continue;

				if (obj->Transform()->IsFrustumCheck() && !m_frustum.CheckFrustumSphere(obj->Transform()->GetWorldPos(), obj->Transform()->GetBoundingRange()))
				{
					continue;
				}

				if (obj->Collider3D())
				{
					m_vecCollider3D.push_back(obj);
				}

				if (obj->Collider2D())
				{
					m_vecCollider2D.push_back(obj);
				}

				if (obj->ParticleSystem())
				{
					m_vecParticle.push_back(obj);
				}
				
				if (obj->Terrain())
				{
					uInstID id = {};
					id.ID = obj->Terrain()->GetInstID();

					map<ULONG64, vector<tInstObj>>::iterator iter = m_mapInstGroup_D.find(id.ID);
					if (iter == m_mapInstGroup_D.end())
					{
						m_mapInstGroup_D.insert(make_pair(id.ID, vector<tInstObj>{ tInstObj{ obj, 0 } }));
					}
					else
					{
						iter->second.push_back(tInstObj{ obj, 0 });
					}
				}

				if (nullptr == obj->MeshRender() || nullptr == obj->MeshRender()->GetMesh())
					continue;

				// 메테리얼 개수만큼 반복
				UINT materialCount = (UINT)obj->MeshRender()->GetMaterialCount();
				for (UINT materialNum = 0; materialNum < materialCount; ++materialNum)
				{
					// Material 을 참조하고 있지 않거나
					// Material 에 아직 Shader 를 연결하지 않은 상태라면 제외
					Ptr<CMaterial> material = obj->MeshRender()->GetSharedMaterial(materialNum);
					if (nullptr == material || material->GetShader() == nullptr)
					{
						continue;
					}

					// Shader 의 POV 에 따라서 인스턴싱 그룹을 분류한다.
					map<ULONG64, vector<tInstObj>>* mapSort = NULL;
					if (material->GetShader()->GetRenderPOV() == RENDER_POV::DEFERRED)
						mapSort = &m_mapInstGroup_D;
					else if (material->GetShader()->GetRenderPOV() == RENDER_POV::FORWARD)
						mapSort = &m_mapInstGroup_F;
					else if (material->GetShader()->GetRenderPOV() == RENDER_POV::POSTEFFECT)
					{
						m_vecPostEffect.push_back(obj);
					}
					else
					{
						assert(nullptr);
						continue;
					}

					uInstID id = {};
					id.ID = obj->MeshRender()->GetInstID(materialNum);

					// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
					if (0 == id.ID)
						continue;

					map<ULONG64, vector<tInstObj>>::iterator iter = mapSort->find(id.ID);
					if (iter == mapSort->end())
					{
						mapSort->insert(make_pair(id.ID, vector<tInstObj>{tInstObj{ obj, materialNum }}));
					}
					else
					{
						iter->second.push_back(tInstObj{ obj, materialNum });
					}
				}
			}
		}
	}
}

void CCamera::CalViewMatrix()
{
	Vec3 pos = Object()->Transform()->GetLocalPos();
	Matrix transMatrix = XMMatrixTranslation(-pos.x, -pos.y, -pos.z);

	Vec3 right = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Matrix orbitMatrix = XMMatrixIdentity();
	orbitMatrix._11 = right.x; orbitMatrix._12 = up.x; orbitMatrix._13 = front.x;
	orbitMatrix._21 = right.y; orbitMatrix._22 = up.y; orbitMatrix._23 = front.y;
	orbitMatrix._31 = right.z; orbitMatrix._32 = up.z; orbitMatrix._33 = front.z;

	m_viewMatrix = transMatrix * orbitMatrix;

	m_viewMatrixInverse = XMMatrixInverse(nullptr, m_viewMatrix);
}

void CCamera::CalProjectionMatrix()
{
	Vec2 renderResolution = CDevice::GetInst()->GetRenderResolution();

	if (PROJECTION_TYPE::PERSPECTIVE == m_projectionType)
		m_projectionMatrix = XMMatrixPerspectiveFovLH(m_FOV, renderResolution.x / renderResolution.y, 1.f, m_far);
	else
		m_projectionMatrix = XMMatrixOrthographicLH(renderResolution.x, renderResolution.y, 1.f, m_far);

	m_projectionMatrixInverse = XMMatrixInverse(nullptr, m_projectionMatrix);
}

void CCamera::CalRay()
{
	CMRT* swapChain = CRenderManager::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT vp = swapChain->GetViewPort();

	Vec2 mousePos = CKeyManager::GetInst()->GetMousePos();

	m_ray.start = Transform()->GetWorldPos();

	m_ray.dir.x = ((((mousePos.x - vp.TopLeftX) * 2.f / vp.Width) - 1.f) - m_projectionMatrix._31) / m_projectionMatrix._11;
	m_ray.dir.y = (-(((mousePos.y - vp.TopLeftY) * 2.f / vp.Height) - 1.f) - m_projectionMatrix._32) / m_projectionMatrix._22;
	m_ray.dir.z = 1.f;

	m_ray.dir = XMVector3TransformNormal(m_ray.dir, m_viewMatrixInverse);
	m_ray.dir.Normalize();
}

void CCamera::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	SaveToFile(&m_projectionType, _file);
	SaveToFile(&m_cameraType, _file);
	SaveToFile(&m_far, _file);
	SaveToFile(&m_FOV, _file);
	SaveToFile(&m_scale, _file);
	SaveToFile(&m_layerCheck, _file);
}

void CCamera::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	LoadFromFile(&m_projectionType, _file);
	LoadFromFile(&m_cameraType, _file);
	LoadFromFile(&m_far, _file);
	LoadFromFile(&m_FOV, _file);
	LoadFromFile(&m_scale, _file);
	LoadFromFile(&m_layerCheck, _file);
}
