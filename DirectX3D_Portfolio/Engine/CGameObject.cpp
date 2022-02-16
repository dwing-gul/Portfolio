#include "pch.h"
#include "CGameObject.h"
#include "CMeshRender.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CLight2D.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CPrefab.h"
#include "CTileMap.h"
#include "CParticleSystem.h"
#include "CScript.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CCamera.h"
#include "CTerrain.h"
#include "CNaviMesh.h"

CGameObject::CGameObject() :
	m_arrComponent{},
	m_ray{},
	m_parent(nullptr),
	m_layerIndex(-1),
	m_meshDataIndex(0),
	m_bDead(false),
	m_objType(OBJECT_TYPE::GENERAL),
	m_bUpdateCheck(true),
	m_bRenderCheck(true),
	m_bOn(true),
	m_bDynamic(false),
	m_bFont(false),
	m_bOffFrame(false)
{
}

CGameObject::CGameObject(const CGameObject& _origin) :
	CEntity(_origin),
	m_arrComponent{},
	m_ray{},
	m_parent(_origin.m_parent),
	m_layerIndex(-1),
	m_meshDataIndex(_origin.m_meshDataIndex),
	m_bDead(false),
	m_objType(_origin.m_objType),
	m_bDynamic(_origin.m_bDynamic),
	m_bUpdateCheck(true),
	m_bRenderCheck(true),
	m_bOn(true),
	m_bFont(false),
	m_bOffFrame(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrComponent[i])
		{
			AddComponent(_origin.m_arrComponent[i]->Clone());
		}
	}

	for (size_t i = 0; i < _origin.m_vecScript.size(); ++i)
	{
		AddComponent(_origin.m_vecScript[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}

	if (Animator3D() && MeshRender()->GetMesh()->IsAnimMesh())
	{
		Animator3D()->SetBones(MeshRender()->GetMesh()->GetBones());
		Animator3D()->SetAnimClip(MeshRender()->GetMesh()->GetAnimClip());
	}
}

CGameObject::~CGameObject()
{
	Safe_Delete_Arr(m_arrComponent);
	Safe_Delete_Vec(m_vecChild);
	Safe_Delete_Vec(m_vecScript);
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComponent[i])
			continue;

		m_arrComponent[i]->start();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->start();
	}
}

void CGameObject::update()
{
	if (!m_bOn && !m_bOffFrame)
		return;

	if (m_layerIndex >= 0 || (m_layerIndex != 0 && m_bUpdateCheck))
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComponent[i])
				continue;

			m_arrComponent[i]->update();
		}

		CalRay();

		for (size_t i = 0; i < m_vecScript.size(); ++i)
		{
			m_vecScript[i]->update();
		}

	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->update();
	}
}

void CGameObject::lateUpdate()
{
	if (!m_bOn && !m_bOffFrame)
		return;

	if (m_layerIndex >= 0 || (m_layerIndex != 0 && m_bUpdateCheck))
	{

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComponent[i])
				continue;

			m_arrComponent[i]->lateUpdate();
		}

		for (size_t i = 0; i < m_vecScript.size(); ++i)
		{
			m_vecScript[i]->lateUpdate();
		}
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->lateUpdate();
	}
}

void CGameObject::finalUpdate()
{
	if (!m_bOn && !m_bOffFrame)
		return;

	if (m_layerIndex >= 0 || (m_layerIndex != 0 && m_bUpdateCheck))
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComponent[i])
				continue;

			m_arrComponent[i]->finalUpdate();
		}
	}
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalUpdate();
	}

	RegisterLayer();

	if (DEFAULT_LAYER < m_layerIndex && UI_LAYER > m_layerIndex)
		m_bUpdateCheck = false;
	else
	{
		m_bUpdateCheck = true;
	}
}

void CGameObject::finalUpdate_ex()
{
	if (!m_bOn)
		return;

	if (m_layerIndex >= 0 || (m_layerIndex != 0 && m_bUpdateCheck))
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComponent[i])
				continue;

			m_arrComponent[i]->finalUpdate();
		}
	}
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalUpdate();
	}
}

void CGameObject::render()
{
	if (!m_bDead)
	{
		if (nullptr != MeshRender())
			MeshRender()->render();

		if (nullptr != Terrain())
			Terrain()->render();

		if (nullptr != TileMap())
			TileMap()->render();

		if (nullptr != ParticleSystem())
			ParticleSystem()->render();

		if (nullptr != Collider2D())
		{
			Collider2D()->render();
		}

		if (nullptr != Collider3D())
		{
			Collider3D()->render();
		}

		if (nullptr != Light2D())
		{
			Light2D()->render();
		}

		if (nullptr != Camera() && Camera() != CRenderManager::GetInst()->GetToolCam())
		{
			Camera()->GetFrustum().render();
		}
	}
}

void CGameObject::render_script(int _index)
{
	m_vecScript[_index]->render();
}

void CGameObject::AddComponent(CComponent* _component)
{
	assert(COMPONENT_TYPE::SCRIPT == _component->GetType() || nullptr == m_arrComponent[(UINT)_component->GetType()]);

	COMPONENT_TYPE type = _component->GetType();

	if (type == COMPONENT_TYPE::MESH_RENDER || type == COMPONENT_TYPE::TILEMAP || type == COMPONENT_TYPE::PARTICLE || type == COMPONENT_TYPE::TERRAIN)
	{
		for (UINT i = (UINT)COMPONENT_TYPE::MESH_RENDER; i < (UINT)COMPONENT_TYPE::PARTICLE + 1; ++i)
		{
			if (type == (COMPONENT_TYPE)i)
				continue;

			if (GetComponent((COMPONENT_TYPE)i))
				assert(nullptr);
		}
	}

	if (_component->GetType() == COMPONENT_TYPE::SCRIPT)
		m_vecScript.push_back((CScript*)_component);
	else
		m_arrComponent[(UINT)_component->GetType()] = _component;

	_component->m_obj = this;
}

void CGameObject::SubComponent(COMPONENT_TYPE _type)
{
	if (nullptr == m_arrComponent[(UINT)_type])
		return;

	delete  m_arrComponent[(UINT)_type];
	m_arrComponent[(UINT)_type] = nullptr;
}

CComponent* CGameObject::GetScriptComponent(UINT eType)
{
	for (size_t i = 0; i < m_vecScript.size(); i++)
	{
		if (eType == m_vecScript[i]->GetScriptType())
			return m_vecScript[i];
	}

	return nullptr;
}

void CGameObject::ChildLayerMove(CGameObject* _parent, int _layerIndex)
{
	DisconnectWithParent();
	CLayer* layer = CSceneManager::GetInst()->GetCurScene()->GetLayer(m_layerIndex);
	layer->DeregisterAsParent(this);

	CScript::CreateObject(this, Transform()->GetLocalPos(), _layerIndex);
	CScript::AddChildByName(_parent->GetName(), this);
}

bool CGameObject::IsAncestor(CGameObject* _obj)
{
	CGameObject* parent = m_parent;
	while (nullptr != parent)
	{
		if (_obj == parent)
		{
			return true;
		}

		parent = parent->GetParent();
	}

	return false;
}

bool CGameObject::IsAncestorByName(const wstring& _name)
{
	CGameObject* parent = m_parent;
	while (nullptr != parent)
	{
		if (_name == parent->GetName())
		{
			return true;
		}

		parent = parent->GetParent();
	}

	return false;
}

void CGameObject::SetDead()
{
	list<CGameObject*> queue;
	queue.push_back(this);

	while (!queue.empty())
	{
		CGameObject* obj = queue.front();
		queue.pop_front();

		obj->m_bDead = true;

		for (size_t i = 0; i < obj->m_vecChild.size(); ++i)
		{
			queue.push_back(obj->m_vecChild[i]);
		}
	}
}

void CGameObject::RegisterLayer()
{
	if (m_bOffFrame)
		m_bOffFrame = false;

	CLayer* layer = CSceneManager::GetInst()->GetCurScene()->GetLayer(m_layerIndex);
	layer->RegisterObj(this);
}

void CGameObject::RegisterAsPrefab(const wstring& _name)
{
	wstring name = _name;
	if (name.empty())
		name = GetName();

	Ptr<CPrefab> pPrefab = new CPrefab(this->Clone());

	CResourceManager::GetInst()->AddRes<CPrefab>(name, pPrefab.Get());
}

void CGameObject::AddChild(CGameObject* _childObj)
{
	assert(false == IsAncestor(_childObj));

	if (_childObj->GetParent() == nullptr && -1 != _childObj->m_layerIndex)
	{
		CScene* curScene = CSceneManager::GetInst()->GetCurScene();
		CLayer* layer = CSceneManager::GetInst()->GetCurScene()->GetLayer(_childObj->m_layerIndex);
		layer->DeregisterAsParent(_childObj);
	}

	_childObj->DisconnectWithParent();

	m_vecChild.push_back(_childObj);
	_childObj->m_parent = this;

	if (-1 == _childObj->m_layerIndex)
	{
		_childObj->m_layerIndex = m_layerIndex;
	}
}

void CGameObject::DisconnectWithParent()
{
	if (nullptr == m_parent)
	{
		return;
	}

	vector<CGameObject*>& vectorChild = m_parent->GetChild_Ref();
	vector<CGameObject*>::iterator iter = vectorChild.begin();

	for (; iter != vectorChild.end(); ++iter)
	{
		if ((*iter) == this)
		{
			vectorChild.erase(iter);
			break;
		}
	}

	m_parent = nullptr;
}

void CGameObject::CalRay()
{
	m_ray.start = Vec3(Transform()->GetWorldPos().x, 0.f, Transform()->GetWorldPos().z);
	m_ray.dir.y = 1.f;

	m_ray.dir.Normalize();
}

void CGameObject::SetOnOff(bool _bOn)
{
	m_bOn = _bOn;
	if (!m_bOn)
		m_bOffFrame = true;

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->m_bOn = _bOn;
		m_vecChild[i]->m_bOffFrame = m_bOffFrame;
	}
}

void CGameObject::SaveToScene(FILE* _file)
{
	CEntity::SaveToScene(_file);

	if (GetParent())
	{
		SaveToFile(&m_layerIndex, _file);
	}

	SaveToFile(&m_objType, _file);
	SaveToFile(&m_bOn, _file);

	UINT component = 0;
	for (; component < (UINT)COMPONENT_TYPE::END; ++component)
	{
		if (nullptr == m_arrComponent[component])
			continue;

		SaveToFile(&component, _file);
		m_arrComponent[component]->SaveToScene(_file);
	}
	SaveToFile(&component, _file);

	UINT scriptCount = (UINT)m_vecScript.size();
	SaveToFile(&scriptCount, _file);

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		CSceneManager::GetInst()->m_saveScript(m_vecScript[i], _file);
	}

	SaveToFile(&m_meshDataIndex, _file);

	UINT childCount = (UINT)m_vecChild.size();
	SaveToFile(&childCount, _file);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->SaveToScene(_file);
	}
}

void CGameObject::LoadFromScene(FILE* _file, int _depth)
{
	CEntity::LoadFromScene(_file);

	if (0 != _depth)
	{
		LoadFromFile(&m_layerIndex, _file);
	}

	LoadFromFile(&m_objType, _file);
	LoadFromFile(&m_bOn, _file);

	UINT componentType = -1;
	while (true)
	{
		LoadFromFile(&componentType, _file);

		if ((UINT)COMPONENT_TYPE::END == componentType)
			break;

		CComponent* component = nullptr;
		switch ((COMPONENT_TYPE)componentType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			component = new CCollider3D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			component = new CAnimator3D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			component = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			component = new CCamera;
			break;
		case COMPONENT_TYPE::LISTENER:
			break;
		case COMPONENT_TYPE::MESH_RENDER:
			component = new CMeshRender;
			break;
		case COMPONENT_TYPE::TERRAIN:
			component = new CTerrain;
			break;
		case COMPONENT_TYPE::TILEMAP:
			component = new CTileMap;
			break;
		case COMPONENT_TYPE::PARTICLE:
			component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::NAVIMESH:
			component = new CNaviMesh;
			break;
		}

		component->LoadFromScene(_file);
		AddComponent(component);
	}

	if (Animator3D() && MeshRender()->GetMesh()->IsAnimMesh())
	{
		Animator3D()->SetBones(MeshRender()->GetMesh()->GetBones());
		Animator3D()->SetAnimClip(MeshRender()->GetMesh()->GetAnimClip());
	}

	UINT scriptCount = 0;
	LoadFromFile(&scriptCount, _file);

	for (UINT i = 0; i < scriptCount; ++i)
	{
		CScript* script = CSceneManager::GetInst()->m_loadScript(_file);
		AddComponent(script);
	}

	++_depth;

	LoadFromFile(&m_meshDataIndex, _file);

	UINT childCount = 0;
	LoadFromFile(&childCount, _file);

	for (UINT i = 0; i < childCount; ++i)
	{
		CGameObject* childObject = new CGameObject;
		childObject->LoadFromScene(_file, _depth);

		AddChild(childObject);
	}
}