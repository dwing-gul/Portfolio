#include "pch.h"
#include "CToolObjectManager.h"
#include "CGameObjectEX.h"
#include "CCameraEX.h"
#include "CToolCamScript.h"
#include "CGridScript.h"
#include <Engine/CRenderManager.h>
#include <Engine/CTransform.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CMaterial.h>

CToolObjectManager::CToolObjectManager() :
	m_gridMaterial(nullptr),
	m_gridShader(nullptr)
{
}

CToolObjectManager::~CToolObjectManager()
{
	Safe_Delete_Vec(m_vecToolObj);
	Safe_Delete_Ptr(m_gridMaterial);
	Safe_Delete_Ptr(m_gridShader);
}

void CToolObjectManager::init()
{
	CGameObjectEX* toolCam = new CGameObjectEX;
	toolCam->SetName(L"ToolCam");
	toolCam->AddComponent(new CTransform);
	toolCam->AddComponent(new CCameraEX);
	toolCam->AddComponent(new CToolCamScript);

	toolCam->Camera()->SetFar(100000.f);
	toolCam->Camera()->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);
	toolCam->Camera()->SetLayerCheckAll();
	m_vecToolObj.push_back(toolCam);
	CRenderManager::GetInst()->SetToolCamera(toolCam->Camera());

	m_gridShader = new CGraphicsShader(RENDER_POV::FORWARD);
	m_gridShader->CreateVertexShader(L"shader\\tool.fx", "VS_Grid");
	m_gridShader->CreatePixelShader(L"shader\\tool.fx", "PS_Grid");
	m_gridShader->SetBlendType(BLEND_TYPE::ALPHABLEND);
	m_gridShader->SetRSType(RASTERIZER_TYPE::CULL_NONE);

	m_gridMaterial = new CMaterial(true);
	m_gridMaterial->SetShader(m_gridShader);

	CGameObjectEX* gridObj = new CGameObjectEX;
	gridObj->SetName(L"Grid");
	gridObj->AddComponent(new CTransform);
	gridObj->AddComponent(new CMeshRender);

	CGridScript* gridScript = new CGridScript;
	gridScript->SetToolCamera(toolCam);
	gridScript->SetGridColor(Vector3(0.2f, 0.8f, 0.2f));
	gridObj->AddComponent(gridScript);

	gridObj->Transform()->SetLocalScale(Vector3(100000.f, 100000.f, 1.f));
	gridObj->Transform()->SetLocalRot(Vector3(XM_PI / 2.f, 0.f, 0.f));
	gridObj->MeshRender()->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gridObj->MeshRender()->SetMaterial(m_gridMaterial);

	m_vecToolObj.push_back(gridObj);
}

void CToolObjectManager::progress()
{
	if (CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		return;

	for (size_t i = 0; i < m_vecToolObj.size(); ++i)
	{
		m_vecToolObj[i]->update();
	}

	for (size_t i = 0; i < m_vecToolObj.size(); ++i)
	{
		m_vecToolObj[i]->lateUpdate();
	}

	for (size_t i = 0; i < m_vecToolObj.size(); ++i)
	{
		m_vecToolObj[i]->finalUpdate();
	}

	for (size_t i = 0; i < m_vecToolObj.size(); ++i)
	{
		m_vecToolObj[i]->render();
	}
}
