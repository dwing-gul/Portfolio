#include "pch.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Collider2D.h"

CollisionManager::CollisionManager() :
    m_arrCheck{}
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::update()
{
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        for (UINT j = 0; j < MAX_LAYER; ++j)
        {
            if (m_arrCheck[i] & (1 << j))
            {
                CollisionLayer(i, j);
            }
        }
    }
}

void CollisionManager::CollisionCheck(UINT _left, UINT _right)
{
    UINT row = _left > _right ? _left : _right;
    UINT col = _left < _right ? _left : _right;

    if (m_arrCheck[col] & (1 << row))
    {
        m_arrCheck[col] &= ~(1 << row);
    }
    else
    {
        m_arrCheck[col] |= (1 << row);
    }
}

void CollisionManager::CollisionLayer(UINT _left, UINT _right)
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	const vector<GameObject*>& left = curScene->GetLayer(_left)->GetAllObj();
	const vector<GameObject*>& right = curScene->GetLayer(_right)->GetAllObj();

	for (size_t i = 0; i < left.size(); ++i)
	{
		if (nullptr == left[i]->GetCollider2D())
			continue;

		for (size_t j = 0; j < right.size(); ++j)
		{
			if (nullptr == right[j]->GetCollider2D() || left[i] == right[j])
				continue;

			ColliderID id = {};
			id.left = left[i]->GetCollider2D()->GetID();
			id.right = right[j]->GetCollider2D()->GetID();

			map<LONGLONG, bool>::iterator iter = m_mapCollisionInfo.find(id.ID);
			if (m_mapCollisionInfo.end() == iter)
			{
				m_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = m_mapCollisionInfo.find(id.ID);
			}

			if (IsCollision(left[i]->GetCollider2D(), right[j]->GetCollider2D()))
			{
				if (iter->second)
				{
					if (left[i]->IsDead() || right[j]->IsDead())
					{
						left[i]->GetCollider2D()->OnColliderExit(right[j]->GetCollider2D());
						right[j]->GetCollider2D()->OnColliderExit(left[i]->GetCollider2D());
						iter->second = false;
					}
					else
					{
						left[i]->GetCollider2D()->OnCollider(right[j]->GetCollider2D());
						right[j]->GetCollider2D()->OnCollider(left[i]->GetCollider2D());
					}
				}
				else
				{
					if (!left[i]->IsDead() && !right[j]->IsDead())
					{
						left[i]->GetCollider2D()->OnColliderEnter(right[j]->GetCollider2D());
						right[j]->GetCollider2D()->OnColliderEnter(left[i]->GetCollider2D());
						iter->second = true;
					}
				}
			}
			else
			{
				if (iter->second)
				{
					left[i]->GetCollider2D()->OnColliderExit(right[j]->GetCollider2D());
					right[j]->GetCollider2D()->OnColliderExit(left[i]->GetCollider2D());
					iter->second = false;
				}
			}
		}
	}
}

bool CollisionManager::IsCollision(Collider2D* _left, Collider2D* _right)
{
	static Vector3 arrLocal[4] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f)
	};

	Vector3 v0 = XMVector3TransformCoord(arrLocal[0], _left->GetWorldMatrix()); v0.z = 0.f;
	Vector3 v1 = XMVector3TransformCoord(arrLocal[1], _left->GetWorldMatrix()); v1.z = 0.f;
	Vector3 v3 = XMVector3TransformCoord(arrLocal[3], _left->GetWorldMatrix()); v3.z = 0.f;

	Vector3 arrAxis[4] = {};

	arrAxis[0] = v1 - v0;
	arrAxis[1] = v3 - v0;

	v0 = XMVector3TransformCoord(arrLocal[0], _right->GetWorldMatrix()); v0.z = 0.f;
	v1 = XMVector3TransformCoord(arrLocal[1], _right->GetWorldMatrix()); v1.z = 0.f;
	v3 = XMVector3TransformCoord(arrLocal[3], _right->GetWorldMatrix()); v3.z = 0.f; 

	arrAxis[2] = v1 - v0;
	arrAxis[3] = v3 - v0;

	Vector3 centerLeft = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _left->GetWorldMatrix()); centerLeft.z = 0.f;
	Vector3 centerRight = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), _right->GetWorldMatrix()); centerRight.z = 0.f;
	Vector3 center = centerRight - centerLeft;

	for (int i = 0; i < 4; ++i)
	{
		Vector3 axis = arrAxis[i];
		axis.Normalize();

		float distance = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			distance += abs(axis.Dot(arrAxis[j])) / 2.f;
		}

		if (distance < abs(axis.Dot(center)))
			return false;
	}

	return true;
}
