#pragma once

class Collider;

union ColliderID
{
	struct
	{
		UINT left;
		UINT right;
	};

	LONGLONG ID;
};
class CollisionManager
{
	SINGLE(CollisionManager)
private :
	UINT m_arrCheck[(UINT)GROUP_TYPE::END];
	map<LONGLONG, bool> m_mapCollisionInfo;

public :
	void update();

	void ColliderCheck(GROUP_TYPE _eLeft, GROUP_TYPE _eRight, bool _check);
	void CollisionClear() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END); }

private :
	void CollisionGroup(int _left, int _right);
	bool IsCollision(Collider* _left, Collider* _right);

};

