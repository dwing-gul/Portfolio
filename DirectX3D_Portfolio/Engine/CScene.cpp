#include "pch.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

CScene::CScene()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_layerIndex = i;
		m_arrLayer[i]->m_ownerScene = this;
	}

	m_arrLayer[0]->SetLayerAction(true);
	m_arrLayer[1]->SetLayerAction(true);
	m_arrLayer[29]->SetLayerAction(true);
	m_arrLayer[30]->SetLayerAction(true);
	m_arrLayer[31]->SetLayerAction(true);
}

CScene::~CScene()
{
	Safe_Delete_Arr(m_arrLayer);
}

void CScene::start()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->start();
	}
}

void CScene::update()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (m_arrLayer[i]->GetLayerAction())
			m_arrLayer[i]->update();
	}
}

void CScene::lateUpdate()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (m_arrLayer[i]->GetLayerAction())
			m_arrLayer[i]->lateUpdate();
	}
}

void CScene::finalUpdate()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (m_arrLayer[i]->GetLayerAction())
			m_arrLayer[i]->finalUpdate();
	}
}

void CScene::AddObject(CGameObject* _obj, int _layerIndex, bool _bWithChild)
{
	assert(!(_layerIndex < 0 || MAX_LAYER <= _layerIndex));

	m_arrLayer[_layerIndex]->AddObject(_obj, _bWithChild);
}

CGameObject* CScene::FindObjByName(const wstring& _name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vectorObj = m_arrLayer[i]->GetParentObj();
		for (size_t i = 0; i < vectorObj.size(); ++i)
		{
			list<CGameObject*> queue;
			queue.push_back(vectorObj[i]);

			while (!queue.empty())
			{
				CGameObject* obj = queue.front();
				queue.pop_front();

				if (_name == obj->GetName())
					return obj;

				vector<CGameObject*> vectorChild = obj->GetChilds();
				for (size_t j = 0; j < vectorChild.size(); ++j)
				{
					queue.push_back(vectorChild[j]);
				}
			}
		}
	}

	return nullptr;
}

vector<CGameObject*> CScene::FindObjByName_Vector(const wstring& _name)
{
	vector<CGameObject*> vecObject;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vectorObj = m_arrLayer[i]->GetParentObj();
		for (size_t k = 0; k < vectorObj.size(); ++k)
		{
			list<CGameObject*> queue;
			queue.push_back(vectorObj[k]);

			while (!queue.empty())
			{
				CGameObject* obj = queue.front();
				queue.pop_front();

				if (_name == obj->GetName())
					vecObject.push_back(obj);

				vector<CGameObject*> vectorChild = obj->GetChilds();
				for (size_t j = 0; j < vectorChild.size(); ++j)
				{
					queue.push_back(vectorChild[j]);
				}
			}
		}
	}

	return vecObject;
}

const vector<CGameObject*>& CScene::FindObjByType(OBJECT_TYPE _type, bool _bChild)
{
	m_vecTarget.clear();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vectorObj = m_arrLayer[i]->GetParentObj();

		if (!_bChild)
		{
			for (size_t j = 0; j < vectorObj.size(); ++j)
			{
				if (_type == vectorObj[j]->GetObjectType())
					m_vecTarget.push_back(vectorObj[j]);
			}
		}
		else
		{
			for (size_t j = 0; j < vectorObj.size(); ++j)
			{
				if (_type == vectorObj[j]->GetObjectType())
					m_vecTarget.push_back(vectorObj[j]);
				SearchObjBytype(vectorObj[j], _type);
			}
		}
	}

	return m_vecTarget;
}

const vector<CGameObject*>& CScene::FindObjByType(OBJECT_TYPE _type, int _layerIndex, bool _bChild)
{
	if (0 > _layerIndex && MAX_LAYER <= _layerIndex)
		assert(nullptr);

	m_vecTarget.clear();

	const vector<CGameObject*>& vectorObj = m_arrLayer[_layerIndex]->GetParentObj();

	if (!_bChild)
	{
		for (size_t i = 0; i < vectorObj.size(); ++i)
		{
			if (_type == vectorObj[i]->GetObjectType())
				m_vecTarget.push_back(vectorObj[i]);
		}
	}
	else
	{
		for (size_t i = 0; i < vectorObj.size(); ++i)
		{
			if (_type == vectorObj[i]->GetObjectType())
				m_vecTarget.push_back(vectorObj[i]);
			SearchObjBytype(vectorObj[i], _type);
		}
	}

	return m_vecTarget;
}

const vector<CGameObject*>& CScene::FindObjByLayerIndex(int _layerIndex, bool _bChild)
{
	if (0 > _layerIndex && MAX_LAYER <= _layerIndex)
		assert(nullptr);

	m_vecTarget.clear();

	const vector<CGameObject*>& vectorObj = m_arrLayer[_layerIndex]->GetParentObj();

	if (!_bChild)
	{
		for (size_t i = 0; i < vectorObj.size(); ++i)
		{
			m_vecTarget.push_back(vectorObj[i]);
		}
	}
	else
	{
		for (size_t j = 0; j < vectorObj.size(); ++j)
		{
			m_vecTarget.push_back(vectorObj[j]);
			SearchChild(vectorObj[j]);
		}
	}

	return m_vecTarget;
}

vector<CGameObject*>& CScene::FindObjByType_Ref(OBJECT_TYPE _type, bool _bChild)
{
	m_vecTarget.clear();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vectorObj = m_arrLayer[i]->GetParentObj();

		if (!_bChild)
		{
			for (size_t j = 0; j < vectorObj.size(); ++j)
			{
				if (_type == vectorObj[j]->GetObjectType())
					m_vecTarget.push_back(vectorObj[j]);
			}
		}
		else
		{
			for (size_t j = 0; j < vectorObj.size(); ++j)
			{
				if (_type == vectorObj[j]->GetObjectType())
					m_vecTarget.push_back(vectorObj[j]);
				SearchObjBytype(vectorObj[j], _type);
			}
		}
	}

	return m_vecTarget;
}

void CScene::DeregisterObject()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->m_vecObj.clear();
	}
}

void CScene::SearchObjBytype(CGameObject* _obj, OBJECT_TYPE _type)
{
	const vector<CGameObject*>& vecChilds = _obj->GetChilds();

	for (size_t i = 0; i < vecChilds.size(); ++i)
	{
		if (_type == vecChilds[i]->GetObjectType())
			m_vecTarget.push_back(vecChilds[i]);

		SearchObjBytype(vecChilds[i], _type);
	}
}

void CScene::SearchChild(CGameObject* _obj)
{
	const vector<CGameObject*>& vecChilds = _obj->GetChilds();

	for (size_t i = 0; i < vecChilds.size(); ++i)
	{
		m_vecTarget.push_back(vecChilds[i]);

		SearchChild(vecChilds[i]);
	}
}

void CScene::SaveToScene(FILE* _file)
{
	CEntity::SaveToScene(_file);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->SaveToScene(_file);
	}
}

void CScene::LoadFromScene(FILE* _file)
{
	CEntity::LoadFromScene(_file);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->LoadFromScene(_file);
	}
}