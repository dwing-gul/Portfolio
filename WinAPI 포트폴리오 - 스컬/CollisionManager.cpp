#include "stdafx.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Obj.h"

CollisionManager::CollisionManager() :
	m_arrCheck{}
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::update()
{
	for (int i = 0; i < (int)GROUP_TYPE::END; ++i)
	{
		for (int j = i; j < (int)GROUP_TYPE::END; ++j)
		{
			if(m_arrCheck[i] & (1 << j))
				CollisionGroup(i, j);
		}
	}
}

void CollisionManager::ColliderCheck(GROUP_TYPE _eLeft, GROUP_TYPE _eRight, bool _check)
{
	UINT row = (UINT)_eLeft > (UINT)_eRight ? (UINT)_eLeft : (UINT)_eRight;
	UINT col = (UINT)_eLeft < (UINT)_eRight ? (UINT)_eLeft : (UINT)_eRight;

	if (_check)
	{
		m_arrCheck[col] |= (1 << row);
	}
	else
	{
		m_arrCheck[col] &= ~(1 << row);
	}
}

void CollisionManager::CollisionGroup(int _left, int _right)
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	vector<Obj*> left = curScene->GetObjects((GROUP_TYPE)_left);
	vector<Obj*> right = curScene->GetObjects((GROUP_TYPE)_right);

	for (size_t i = 0; i < left.size(); ++i)
	{
		if (nullptr == left[i]->GetCollider())
			continue;

		for (size_t j = 0; j < right.size(); ++j)
		{
			if (nullptr == right[j]->GetCollider() || left[i] == right[j])
				continue;

			ColliderID id = {};
			id.left = left[i]->GetCollider()->GetID();
			id.right = right[j]->GetCollider()->GetID();

			map<LONGLONG, bool>::iterator iter = m_mapCollisionInfo.find(id.ID);
			if (m_mapCollisionInfo.end() == iter)
			{
				m_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = m_mapCollisionInfo.find(id.ID);
			}

			if (IsCollision(left[i]->GetCollider(), right[j]->GetCollider()))
			{
				if (left[i]->IsDead() || right[j]->IsDead())
				{
					left[i]->GetCollider()->OnColliderExit(right[j]->GetCollider());
					right[j]->GetCollider()->OnColliderExit(left[i]->GetCollider());
					iter->second = false;
				}
				else
				{
					if (!(iter->second))
					{
						left[i]->GetCollider()->OnColliderEnter(right[j]->GetCollider());
						right[j]->GetCollider()->OnColliderEnter(left[i]->GetCollider());
						iter->second = true;
					}
					else
					{
						left[i]->GetCollider()->OnCollider(right[j]->GetCollider());
						right[j]->GetCollider()->OnCollider(left[i]->GetCollider());
					}
				}
			}
			else
			{
				if (iter->second)
				{
					left[i]->GetCollider()->OnColliderExit(right[j]->GetCollider());
					right[j]->GetCollider()->OnColliderExit(left[i]->GetCollider());
					iter->second = false;
				}
			}
		}
	}
}

bool CollisionManager::IsCollision(Collider* _left, Collider* _right)
{
	if((_left->GetScale().x + _right->GetScale().x) / 2.f < abs(_left->GetFinalPos().x - _right->GetFinalPos().x))
		return false;
	if((_left->GetScale().y + _right->GetScale().y) / 2.f < abs(_left->GetFinalPos().y - _right->GetFinalPos().y))
		return false;

	return true;
}
