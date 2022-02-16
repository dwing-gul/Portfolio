#include "pch.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CCollider3D.h"

CCollisionManager::CCollisionManager() :
	m_arrCheck{}
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if (m_arrCheck[i] & (1 << j))
			{
				Collision3DLayer(i, j);
				//CollisionLayer(i, j);
			}
		}
	}
}

void CCollisionManager::CollisionCheck(UINT _left, UINT _right)
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

void CCollisionManager::CollisionLayer(UINT _left, UINT _right)
{
	CScene* curScene = CSceneManager::GetInst()->GetCurScene();

	const vector<CGameObject*>& left = curScene->GetLayer(_left)->GetAllObj();
	const vector<CGameObject*>& right = curScene->GetLayer(_right)->GetAllObj();

	for (size_t i = 0; i < left.size(); ++i)
	{
		if (nullptr == left[i]->Collider2D())
			continue;

		for (size_t j = 0; j < right.size(); ++j)
		{
			if (nullptr == right[j]->Collider2D() || left[i] == right[j])
				continue;

			uColliderID id = {};
			id.left = left[i]->Collider2D()->GetID();
			id.right = right[j]->Collider2D()->GetID();

			map<LONGLONG, bool>::iterator iter = m_mapCollisionInfo.find(id.ID);
			if (m_mapCollisionInfo.end() == iter)
			{
				m_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = m_mapCollisionInfo.find(id.ID);
			}

			if (IsCollision(left[i]->Collider2D(), right[j]->Collider2D()))
			{
				if (iter->second)
				{
					if (left[i]->IsDead() || right[j]->IsDead())
					{
						left[i]->Collider2D()->OnColliderExit(right[j]->Collider2D());
						right[j]->Collider2D()->OnColliderExit(left[i]->Collider2D());
						iter->second = false;
					}
					else
					{
						left[i]->Collider2D()->OnCollider(right[j]->Collider2D());
						right[j]->Collider2D()->OnCollider(left[i]->Collider2D());
					}
				}
				else
				{
					if (!left[i]->IsDead() && !right[j]->IsDead())
					{
						left[i]->Collider2D()->OnColliderEnter(right[j]->Collider2D());
						right[j]->Collider2D()->OnColliderEnter(left[i]->Collider2D());
						iter->second = true;
					}

				}
			}
			else
			{
				if (iter->second)
				{
					left[i]->Collider2D()->OnColliderExit(right[j]->Collider2D());
					right[j]->Collider2D()->OnColliderExit(left[i]->Collider2D());
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionManager::IsCollision(CCollider2D* _left, CCollider2D* _right)
{
	static Vec3 arrLocal[4] =
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};

	Vec3 v0 = XMVector3TransformCoord(arrLocal[0], _left->GetWorldMatrix()); v0.z = 0.f;
	Vec3 v1 = XMVector3TransformCoord(arrLocal[1], _left->GetWorldMatrix()); v1.z = 0.f;
	Vec3 v3 = XMVector3TransformCoord(arrLocal[3], _left->GetWorldMatrix()); v3.z = 0.f;

	Vec3 arrAsix[4] = {};

	arrAsix[0] = v1 - v0;
	arrAsix[1] = v3 - v0;

	v0 = XMVector3TransformCoord(arrLocal[0], _right->GetWorldMatrix()); v0.z = 0.f;
	v1 = XMVector3TransformCoord(arrLocal[1], _right->GetWorldMatrix()); v1.z = 0.f;
	v3 = XMVector3TransformCoord(arrLocal[3], _right->GetWorldMatrix()); v3.z = 0.f;

	arrAsix[2] = v1 - v0;
	arrAsix[3] = v3 - v0;

	Vec3 centerLeft = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _left->GetWorldMatrix());
	Vec3 centerRight = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _right->GetWorldMatrix());
	centerLeft.z = 0.f; centerRight.z = 0.f;
	Vec3 center = centerRight - centerLeft;

	for (int i = 0; i < 4; ++i)
	{
		Vec3 asix = arrAsix[i];
		asix.Normalize();

		float distance = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			distance += abs(asix.Dot(arrAsix[j])) / 2.f;
		}

		if (distance < abs(asix.Dot(center)))
			return false;
	}

	return true;
}

void CCollisionManager::Collision3DLayer(UINT _left, UINT _right)
{
	CScene* curScene = CSceneManager::GetInst()->GetCurScene();

	const vector<CGameObject*>& left = curScene->GetLayer(_left)->GetAllObj();
	const vector<CGameObject*>& right = curScene->GetLayer(_right)->GetAllObj();

	for (size_t i = 0; i < left.size(); ++i)
	{
		if (nullptr == left[i]->Collider3D())
			continue;

		for (size_t j = 0; j < right.size(); ++j)
		{
			if (nullptr == right[j]->Collider3D() || left[i] == right[j])
				continue;

			uColliderID id = {};

			UINT leftID = left[i]->Collider3D()->GetID();
			UINT rightID = right[j]->Collider3D()->GetID();

			id.left = leftID < rightID ? leftID : rightID;
			id.right = leftID < rightID ? rightID : leftID;
			
			map<LONGLONG, bool>::iterator iter = m_mapCollisionInfo.find(id.ID);
			if (m_mapCollisionInfo.end() == iter)
			{
				m_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = m_mapCollisionInfo.find(id.ID);
			}

			if (IsCollision3D(left[i]->Collider3D(), right[j]->Collider3D()))
			{
				if (iter->second)
				{
					if (left[i]->IsDead() || right[j]->IsDead() || !left[i]->IsOn() || !right[j]->IsOn())
					{
						left[i]->Collider3D()->OnColliderExit(right[j]->Collider3D());
						right[j]->Collider3D()->OnColliderExit(left[i]->Collider3D());
						iter->second = false;
					}
					else
					{
						left[i]->Collider3D()->OnCollider(right[j]->Collider3D());
						right[j]->Collider3D()->OnCollider(left[i]->Collider3D());
					}
				}
				else
				{
					if (!left[i]->IsDead() && !right[j]->IsDead() && left[i]->IsOn() && right[j]->IsOn())
					{
						left[i]->Collider3D()->OnColliderEnter(right[j]->Collider3D());
						right[j]->Collider3D()->OnColliderEnter(left[i]->Collider3D());
						iter->second = true;
					}

				}
			}
			else
			{
				if (iter->second)
				{
					left[i]->Collider3D()->OnColliderExit(right[j]->Collider3D());
					right[j]->Collider3D()->OnColliderExit(left[i]->Collider3D());
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionManager::IsCollision3D(CCollider3D* _left, CCollider3D* _right)
{
	// 큐브를 이루는 정점
	static Vec3 arrLocal[8] =
	{
		// 윗면
		Vec3(-0.5f, 0.5f, 0.5f), // 0
		Vec3(0.5f, 0.5f, 0.5f), // 1
		Vec3(0.5f, 0.5f, -0.5f), // 2
		Vec3(-0.5f, 0.5f, -0.5f), // 3

		// 아랫면
		Vec3(-0.5f, -0.5f, -0.5f), //4
		Vec3(0.5f, -0.5f, -0.5f), // 5
		Vec3(0.5f, -0.5f, 0.5f), // 6
		Vec3(-0.5f, -0.5f, 0.5f) //7
	};
	
	// 구와 큐브의 충돌을 할 때 구를 감쌀 정점
	static Vec3 arrLocalForSphere[8] =
	{
		// 윗면
		Vec3(-1.f, 1.f, 1.f), // 0
		Vec3(1.f, 1.f, 1.f), // 1
		Vec3(1.f, 1.f, -1.f), // 2
		Vec3(-1.f, 1.f, -1.f), // 3

		// 아랫면
		Vec3(-1.f, -1.f, -1.f), //4
		Vec3(1.f, -1.f, -1.f), // 5
		Vec3(1.f, -1.f, 1.f), // 6
		Vec3(-1.f, -1.f, 1.f) //7
	};

	// 충돌 계산을 위한 축
	Vec3 arrAxis[15] = {};

	Vec3 leftEdge[3] = {};
	Vec3 rightEdge[3] = {};

	// 각 육면체의 중심과 두 육면체의 중심 사이의 거리
	Vec3 centerLeft = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _left->GetWorldMatrix());
	Vec3 centerRight = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _right->GetWorldMatrix());
	Vec3 center = centerRight - centerLeft;

	// 분리 축은 모든 면에 대한 법선 벡터, 두 육면체에서 각각 한 모서리 씩 가져와서 외적한 벡터, 

	Vec3 v0;
	Vec3 v1;
	Vec3 v3;
	Vec3 v7;
	//두 육면체에서 각각 한 모서리 씩 가져와서 외적한 벡터
	if (_left->IsSphere())
	{
		v0 = XMVector3TransformCoord(arrLocalForSphere[0], _left->GetWorldMatrix());
		v1 = XMVector3TransformCoord(arrLocalForSphere[1], _left->GetWorldMatrix());
		v3 = XMVector3TransformCoord(arrLocalForSphere[3], _left->GetWorldMatrix());
		v7 = XMVector3TransformCoord(arrLocalForSphere[7], _left->GetWorldMatrix());
	}
	else
	{
		v0 = XMVector3TransformCoord(arrLocal[0], _left->GetWorldMatrix());
		v1 = XMVector3TransformCoord(arrLocal[1], _left->GetWorldMatrix());
		v3 = XMVector3TransformCoord(arrLocal[3], _left->GetWorldMatrix());
		v7 = XMVector3TransformCoord(arrLocal[7], _left->GetWorldMatrix());
	}

	// 육면체의 중심에서 임의의 한 꼭짓점에 이르는 거리
	Vec3 leftExtent = v0 - centerLeft;

	leftEdge[0] = v1 - v0;
	leftEdge[1] = v3 - v0;
	leftEdge[2] = v7 - v0;

	if (_right->IsSphere())
	{
		v0 = XMVector3TransformCoord(arrLocalForSphere[0], _right->GetWorldMatrix());
		v1 = XMVector3TransformCoord(arrLocalForSphere[1], _right->GetWorldMatrix());
		v3 = XMVector3TransformCoord(arrLocalForSphere[3], _right->GetWorldMatrix());
		v7 = XMVector3TransformCoord(arrLocalForSphere[7], _right->GetWorldMatrix());
	}
	else
	{
		v0 = XMVector3TransformCoord(arrLocal[0], _right->GetWorldMatrix());
		v1 = XMVector3TransformCoord(arrLocal[1], _right->GetWorldMatrix());
		v3 = XMVector3TransformCoord(arrLocal[3], _right->GetWorldMatrix());
		v7 = XMVector3TransformCoord(arrLocal[7], _right->GetWorldMatrix());
	}
	
	// 각 육면체의 중심에서 임의의 한 꼭짓점에 이르는 거리
	Vec3 rightExtent = v0 - centerRight;

	rightEdge[0] = v1 - v0;
	rightEdge[1] = v3 - v0;
	rightEdge[2] = v7 - v0;

	arrAxis[0] = leftEdge[0].Cross(rightEdge[0]);
	arrAxis[1] = leftEdge[0].Cross(rightEdge[1]);
	arrAxis[2] = leftEdge[0].Cross(rightEdge[2]);
	arrAxis[3] = leftEdge[1].Cross(rightEdge[0]);
	arrAxis[4] = leftEdge[1].Cross(rightEdge[1]);
	arrAxis[5] = leftEdge[1].Cross(rightEdge[2]);
	arrAxis[6] = leftEdge[2].Cross(rightEdge[0]);
	arrAxis[7] = leftEdge[2].Cross(rightEdge[1]);
	arrAxis[8] = leftEdge[2].Cross(rightEdge[2]);

	//모든 면에 대한 법선 벡터
	arrAxis[9] = leftEdge[0];
	arrAxis[10] = leftEdge[1];
	arrAxis[11] = leftEdge[2];
	arrAxis[12] = rightEdge[0];
	arrAxis[13] = rightEdge[1];
	arrAxis[14] = rightEdge[2];

	// 충돌 계산
	for (int i = 0; i < 15; ++i)
	{
		Vec3 axis = arrAxis[i];
		axis.Normalize();

		float distance = 0.f;

		// 분리축 후보에 각 육면체의 중심에서 꼭짓점까지의 거리를 투영시킨 거리의 합이 중심끼리를 이은 것을 투영시킨 거리 보다 짧으면 분리축이다.
		distance = abs(axis.Dot(leftExtent)) + abs(axis.Dot(rightExtent));

		if (distance < abs(axis.Dot(center)))
			return false;
	}

	if (_left->IsSphere() || _right->IsSphere())
	{
		float distance = 0.f;

		// 분리축 후보에 각 육면체의 중심에서 꼭짓점까지의 거리를 투영시킨 거리의 합이 중심끼리를 이은 것을 투영시킨 거리 보다 짧으면 분리축이다.
		distance = abs(_left->GetWorldOffSetScale().x + _right->GetWorldOffSetScale().x);

		if (distance < Vec3::Distance(centerLeft, centerRight))
			return false;
	}

	return true;
}