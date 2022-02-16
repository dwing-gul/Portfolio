#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CScene.h"

CLayer::CLayer() :
	m_ownerScene(nullptr),
	m_layerIndex(-1),
	m_layerAction(false)
{
}

CLayer::~CLayer()
{
	Safe_Delete_Vec(m_vecParentObj);
}

void CLayer::start()
{
	for (size_t i = 0; i < m_vecParentObj.size(); ++i)
	{
		m_vecParentObj[i]->start();
	}
}

void CLayer::update()
{
	for (size_t i = 0; i < m_vecParentObj.size(); ++i)
	{
		m_vecParentObj[i]->update();
	}
}

void CLayer::lateUpdate()
{
	for (size_t i = 0; i < m_vecParentObj.size(); ++i)
	{
		m_vecParentObj[i]->lateUpdate();
	}
}

void CLayer::finalUpdate()
{
	vector<CGameObject*>::iterator iter = m_vecParentObj.begin();

	for (; iter != m_vecParentObj.end();)
	{
		(*iter)->finalUpdate();

		if ((*iter)->IsDead())
		{
			iter = m_vecParentObj.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::AddObject(CGameObject* _obj, bool _bWithChild)
{
	assert(m_layerIndex != _obj->m_layerIndex);

	if (0 <= _obj->m_layerIndex && _obj->m_layerIndex < MAX_LAYER)
	{
		m_ownerScene->GetLayer(_obj->m_layerIndex)->ResignObject(_obj);
	}

	m_vecParentObj.push_back(_obj);


	list<CGameObject*> queue;
	queue.push_back(_obj);

	while (!queue.empty())
	{
		CGameObject* obj = queue.front();
		queue.pop_front();

		if (_bWithChild || -1 == obj->m_layerIndex)
			obj->m_layerIndex = m_layerIndex;

		for (size_t i = 0; i < obj->m_vecChild.size(); ++i)
		{
			queue.push_back(obj->m_vecChild[i]);
		}
	}
}

void CLayer::DeregisterAsParent(CGameObject* _obj)
{
	vector<CGameObject*>::iterator iter = m_vecParentObj.begin();
	for (; iter != m_vecParentObj.end(); ++iter)
	{
		if (_obj == *iter)
		{
			m_vecParentObj.erase(iter);
			return;
		}
	}

	return;
}

void CLayer::ResignObject(CGameObject* _obj)
{
	vector<CGameObject*>::iterator iter = m_vecParentObj.begin();
	for (; iter != m_vecParentObj.end(); ++iter)
	{
		if ((*iter) == _obj)
		{
			m_vecParentObj.erase(iter);
			break;
		}
	}

	list<CGameObject*> queue;
	queue.push_back(_obj);

	while (!queue.empty())
	{
		CGameObject* obj = queue.front();
		queue.pop_front();

		obj->m_layerIndex = -1;

		for (size_t i = 0; i < obj->m_vecChild.size(); ++i)
		{
			queue.push_back(obj->m_vecChild[i]);
		}
	}
}

void CLayer::SaveToScene(FILE* _file)
{
	CEntity::SaveToScene(_file);

	UINT objCount = (UINT)m_vecParentObj.size();
	SaveToFile(&objCount, _file);

	for (size_t i = 0; i < m_vecParentObj.size(); ++i)
	{
		m_vecParentObj[i]->SaveToScene(_file);
	}
}

void CLayer::LoadFromScene(FILE* _file)
{
	CEntity::LoadFromScene(_file);

	UINT objCount = 0;
	LoadFromFile(&objCount, _file);

	CGameObject* obj = nullptr;
	for (UINT i = 0; i < objCount; ++i)
	{
		obj = new CGameObject;
		obj->LoadFromScene(_file, 0);
		AddObject(obj, false);
	}
}