#include "pch.h"
#include "CLightCamera.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CAnimator3D.h"
#include "CTerrain.h"
#include "CInstancingBuffer.h"

CLightCamera::CLightCamera()
{
}

CLightCamera::~CLightCamera()
{
}

void CLightCamera::finalUpdate()
{
	CalViewMatrix();
	CalProjectionMatrix();
	CalRay();
}

void CLightCamera::SortShadowObject()
{
	m_mapInstGroup_S.clear();

	CScene* curScene = CSceneManager::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_layerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = curScene->GetLayer(i)->GetAllObj();

			for (size_t j = 0; j < vecObj.size(); ++j)
			{
				if (!vecObj[j]->GetRenderCheck() || !vecObj[j]->IsOn())
					continue;

				if (vecObj[j]->Transform()->IsFrustumCheck() && !m_frustum.CheckFrustumSphere(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetBoundingRange()))
				{
					continue;
				}

				if (vecObj[j]->ParticleSystem())
				{
					m_vecParticle.push_back(vecObj[j]);
					continue;
				}

				if (!vecObj[j]->IsDynamic() || nullptr == vecObj[j]->MeshRender() || nullptr == vecObj[j]->MeshRender()->GetMesh())
					continue;

				UINT materialCount = (UINT)vecObj[j]->MeshRender()->GetMaterialCount();
				for (UINT materialNum = 0; materialNum < materialCount; ++materialNum)
				{
					Ptr<CMaterial> material = vecObj[j]->MeshRender()->GetSharedMaterial(materialNum);

					if (nullptr == material || material->GetShader() == nullptr)
						continue;

					uInstID ID = {};
					ID.ID = vecObj[j]->MeshRender()->GetInstShadowID(materialNum);

					if (0 == ID.ID)
						continue;

					map<ULONG64, vector<tInstObj>>::iterator iter = m_mapInstGroup_S.find(ID.ID);
					if (iter == m_mapInstGroup_S.end())
					{
						m_mapInstGroup_S.insert(make_pair(ID.ID, vector<tInstObj>{tInstObj{ vecObj[j], materialNum }}));
					}
					else
					{
						iter->second.push_back(tInstObj{ vecObj[j], materialNum });
					}
				}
			}
		}
	}
}

void CLightCamera::render_shadowdepth()
{
	// 뷰행렬과 투영행렬을 광원시점 카메라의 것으로 대체해둠
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.viewMatrixInverse = m_viewMatrixInverse;
	g_transform.projectionMatrix = m_projectionMatrix;

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_S)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1 || pair.second[0].obj->Animator2D() || 
			pair.second[0].obj->MeshRender()->GetSharedMaterial(pair.second[0].materialIndex)->GetShader()->GetInstVS() == nullptr || 
			pair.second[0].obj->Terrain())
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].obj);

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
		Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(L"ShadowDepthMaterial");

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

		// 위치정보 업데이트
		pair.second[0].obj->Transform()->UpdateData();

		// 재질 반복 돌면서 그림자 깊이 그리기
		for (auto& instObj : pair.second)
		{
			if (instObj.obj->Terrain())
			{
				instObj.obj->Terrain()->render();
			}
			else
			{
				instObj.obj->MeshRender()->render_shadowdepth(instObj.materialIndex);
			}
		}

		// Animatator3D 정보 해제
		if (pair.second[0].obj->Animator3D())
		{
			pair.second[0].obj->Animator3D()->ClearData();
		}
	}
}
