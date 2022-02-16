#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRender.h"
#include "GameObject.h"
#include "Device.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

Camera::Camera() :
	Component(COMPONENT_TYPE::CAMERA),
	m_projectionType(PROJECTION_TYPE::PERSPECTIVE),
	m_far(10000.f),
	m_FOV(XM_PI / 3.f),
	m_scale(1.f),
	m_layerCheck(0)
{
	m_viewMatrix = XMMatrixIdentity();
	m_projectionMatrix = XMMatrixIdentity();
}

Camera::~Camera()
{
}

void Camera::finalUpdate()
{
	CalViewMatrix();
	CalProjectionMatrix();

	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	RenderManager::GetInst()->RegisterCamera(this);
}

void Camera::render_tilemap()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	for (size_t i = 0; i < m_vectorTileMap.size(); ++i)
	{
		m_vectorTileMap[i]->render();
	}
}

void Camera::render_forward()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	for (size_t i = 0; i < m_vectorForward.size(); ++i)
	{
		m_vectorForward[i]->render();
	}
}

void Camera::render_particle()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	for (size_t i = 0; i < m_vectorParticle.size(); ++i)
	{
		m_vectorParticle[i]->render();
	}
}

void Camera::render_posteffect()
{
	g_transform.viewMatrix = m_viewMatrix;
	g_transform.projectionMatrix = m_projectionMatrix;

	for (size_t i = 0; i < m_vectorPostEffect.size(); ++i)
	{
		RenderManager::GetInst()->CopyBackBuffer();
		m_vectorPostEffect[i]->render();
	}
}

void Camera::SortObject()
{
	m_vectorTileMap.clear();
	m_vectorForward.clear();
	m_vectorParticle.clear();
	m_vectorPostEffect.clear();

	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if (nullptr == curScene)
		return;

	if (SceneManager::GetInst()->FindObjByName(L"Light"))
	{
		int a = 0;
	}

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (m_layerCheck & (1 << i))
		{
			Layer* layer = curScene->GetLayer(i);
			const vector<GameObject*>& vectorAllObj = layer->GetAllObj();
			for (size_t j = 0; j < vectorAllObj.size(); ++j)
			{
				GameObject* obj = vectorAllObj[j];

				if (obj->GetMeshRender() && obj->GetMeshRender()->GetSharedMaterial().Get() && obj->GetMeshRender()->GetSharedMaterial()->GetShader().Get())
				{
					Ptr<GraphicsShader> shader = obj->GetMeshRender()->GetSharedMaterial()->GetShader();

					if (RENDER_POV::FORWARD == shader->GetRenderPOV())
					{
						m_vectorForward.push_back(obj);
					}
					else if (RENDER_POV::POST_EFFECT == shader->GetRenderPOV())
					{
						m_vectorPostEffect.push_back(obj);
					}
				}
				else if (obj->GetParticle())
				{
					m_vectorParticle.push_back(obj);
				}
				else if (obj->GetTileMap())
				{
					m_vectorTileMap.push_back(obj);
				}

			}
		}
	}
}

void Camera::CalViewMatrix()
{
	Vector3 pos = Object()->GetTransform()->GetLocalPos();
	Matrix transMatrix = XMMatrixTranslation(-pos.x, -pos.y, -pos.z);

	Vector3 right = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vector3 up = GetTransform()->GetWorldDir(DIR_TYPE::UP);
	Vector3 front = GetTransform()->GetWorldDir(DIR_TYPE::FRONT);

	Matrix orbitMatrix = XMMatrixIdentity();
	orbitMatrix._11 = right.x; orbitMatrix._12 = up.x; orbitMatrix._13 = front.x;
	orbitMatrix._21 = right.y; orbitMatrix._22 = up.y; orbitMatrix._23 = front.y;
	orbitMatrix._31 = right.z; orbitMatrix._32 = up.z; orbitMatrix._33 = front.z;

	m_viewMatrix = transMatrix * orbitMatrix;
}

void Camera::CalProjectionMatrix()
{
	Vector2 resolution = Device::GetInst()->GetRenderResolution();

	if (PROJECTION_TYPE::PERSPECTIVE == m_projectionType)
	{
		m_projectionMatrix = XMMatrixPerspectiveFovLH(m_FOV, resolution.x / resolution.y, 1.f, m_far);
	}
	else
	{
		m_projectionMatrix = XMMatrixOrthographicLH(resolution.x * m_scale, resolution.y * m_scale, 1.f, m_far);
	}
}

void Camera::SaveToScene(FILE* _file)
{
	Component::SaveToScene(_file);
	SaveToFile(&m_projectionType, _file);
	SaveToFile(&m_far, _file);
	SaveToFile(&m_FOV, _file);
	SaveToFile(&m_scale, _file);
	SaveToFile(&m_layerCheck, _file);
}

void Camera::LoadFromScene(FILE* _file)
{
	Component::LoadFromScene(_file);
	LoadFromFile(&m_projectionType, _file);
	LoadFromFile(&m_far, _file);
	LoadFromFile(&m_FOV, _file);
	LoadFromFile(&m_scale, _file);
	LoadFromFile(&m_layerCheck, _file);
}
