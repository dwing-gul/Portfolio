#include "pch.h"
#include "Layer.h"
#include "Scene.h"
#include "GameObject.h"

Layer::Layer() :
	m_ownerScene(nullptr),
	m_layerIndex(-1)
{
}

Layer::~Layer()
{
	Safe_Delete_Vector(m_vectorParentObj);
}

void Layer::start()
{
	for (size_t i = 0; i < m_vectorParentObj.size(); ++i)
	{
		m_vectorParentObj[i]->start();
	}
}

void Layer::update()
{
	for (size_t i = 0; i < m_vectorParentObj.size(); ++i)
	{
		m_vectorParentObj[i]->update();
	}
}

void Layer::lateUpdate()
{
	for (size_t i = 0; i < m_vectorParentObj.size(); ++i)
	{
		m_vectorParentObj[i]->lateUpdate();
	}
}

void Layer::finalUpdate()
{
	vector<GameObject*>::iterator iter = m_vectorParentObj.begin();

	for (; iter != m_vectorParentObj.end();)
	{
		(*iter)->finalUpdate();

		if ((*iter)->IsDead())
		{
			iter = m_vectorParentObj.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}


void Layer::AddObj(GameObject* _obj, bool _bWithChild)
{
	assert(m_layerIndex != _obj->m_layerIndex);

	if (0 <= _obj->m_layerIndex && _obj->m_layerIndex < MAX_LAYER)
	{
		m_ownerScene->GetLayer(_obj->m_layerIndex)->ResignObj(_obj);
	}

	m_vectorParentObj.push_back(_obj);

	list<GameObject*> queue;
	queue.push_back(_obj);

	while (!queue.empty())
	{
		GameObject* obj = queue.front();
		queue.pop_front();

		if (_bWithChild || -1 == obj->m_layerIndex)
		{
			obj->m_layerIndex = m_layerIndex;
		}

		for (size_t i = 0; i < obj->m_vectorChild.size(); ++i)
		{
			queue.push_back(obj->m_vectorChild[i]);
		}
	}
}

void Layer::DeregisterObj(GameObject* _obj)
{
	vector<GameObject*>::iterator iter = m_vectorParentObj.begin();
	for (; iter != m_vectorParentObj.end(); ++iter)
	{
		if (_obj == *iter)
		{
			m_vectorParentObj.erase(iter);
			return;
		}
	}

	return;
	assert(nullptr);
}

void Layer::ResignObj(GameObject* _obj)
{
	vector<GameObject*>::iterator iter = m_vectorParentObj.begin();
	for (; iter != m_vectorParentObj.end(); ++iter)
	{
		if ((*iter) == _obj)
		{
			m_vectorParentObj.erase(iter);
			break;
		}
	}

	list<GameObject*> queue;
	queue.push_back(_obj);

	while (!queue.empty())
	{
		GameObject* obj = queue.front();
		queue.pop_front();

		
		obj->m_layerIndex = -1;

		for (size_t i = 0; i < obj->m_vectorChild.size(); ++i)
		{
			queue.push_back(obj->m_vectorChild[i]);
		}
	}
}

void Layer::SaveToScene(FILE* _file)
{
	Entity::SaveToScene(_file);

	UINT objCount = (UINT)m_vectorParentObj.size();
	SaveToFile(&objCount, _file);

	for (size_t i = 0; i < m_vectorParentObj.size(); ++i)
	{
		m_vectorParentObj[i]->SaveToScene(_file);
	}
}

void Layer::LoadFromScene(FILE* _file)
{
	Entity::LoadFromScene(_file);

	UINT objCount = 0;
	LoadFromFile(&objCount, _file);

	GameObject* obj = nullptr;
	for (UINT i = 0; i < objCount; ++i)
	{
		obj = new GameObject;
		obj->LoadFromScene(_file, 0);
		AddObj(obj, false);
	}
}