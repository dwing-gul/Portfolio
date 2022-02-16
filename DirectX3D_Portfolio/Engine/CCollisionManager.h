#pragma once

class CCollider2D;
class CCollider3D;

union uColliderID
{
	struct
	{
		UINT left;
		UINT right;
	};

	LONGLONG ID;
};

class CCollisionManager :
	public CSingleton<CCollisionManager>
{
	SINGLE(CCollisionManager);
private:
	UINT m_arrCheck[MAX_LAYER];
	map<LONGLONG, bool> m_mapCollisionInfo;

public:
	void update();

	void CollisionCheck(UINT _left, UINT _right);
	void CollisionClear()
	{
		memset(m_arrCheck, 0, sizeof(UINT) * MAX_LAYER);
		m_mapCollisionInfo.clear();
	}

private:
	void CollisionLayer(UINT _left, UINT _right);
	bool IsCollision(CCollider2D* _left, CCollider2D* _right);

	void Collision3DLayer(UINT _left, UINT _right);
	bool IsCollision3D(CCollider3D* _left, CCollider3D* _right);
};
