#pragma once

class Collider2D;

union ColliderID
{
	struct
	{
		UINT left;
		UINT right;
	};

	LONGLONG ID;
};

class CollisionManager :
	public Singleton<CollisionManager>
{
	SINGLE(CollisionManager)
private :
	UINT				m_arrCheck[MAX_LAYER];
	map<LONGLONG, bool> m_mapCollisionInfo;

public :
	void update();
	void CollisionCheck(UINT _left, UINT _right);
	void CollisionClear()
	{
		memset(m_arrCheck, 0, sizeof(UINT) * MAX_LAYER);
		m_mapCollisionInfo.clear();
	}

private :
	void CollisionLayer(UINT _left, UINT _right);
	bool IsCollision(Collider2D* _left, Collider2D* _right);
};