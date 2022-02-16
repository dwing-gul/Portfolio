#include "pch.h"
#include "ToolObjectManager.h"
#include "GameObjectEX.h"
#include "CameraEX.h"
#include "ToolCamScript.h"
#include <Engine/RenderManager.h>
#include <Engine/Transform.h>

ToolObjectManager::ToolObjectManager()
{
}

ToolObjectManager::~ToolObjectManager()
{
	Safe_Delete_Vector(m_vectorToolObj);
}

void ToolObjectManager::init()
{
	GameObjectEX* toolCam = new GameObjectEX;
	toolCam->AddComponent(new Transform);
	toolCam->AddComponent(new CameraEX);
	toolCam->AddComponent(new ToolCamScript);

	toolCam->GetCamera()->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);
	toolCam->GetCamera()->SetLayerCheckAll();

	m_vectorToolObj.push_back(toolCam);
	RenderManager::GetInst()->SetToolCamera(toolCam->GetCamera());
}

void ToolObjectManager::progress()
{
	for (size_t i = 0; i < m_vectorToolObj.size(); ++i)
	{
		m_vectorToolObj[i]->update();
	}

	for (size_t i = 0; i < m_vectorToolObj.size(); ++i)
	{
		m_vectorToolObj[i]->lateUpdate();
	}

	for (size_t i = 0; i < m_vectorToolObj.size(); ++i)
	{
		m_vectorToolObj[i]->finalUpdate();
	}
}
