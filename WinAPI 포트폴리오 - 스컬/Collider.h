#pragma once

class Obj;

class Collider
{
private :
	static UINT g_ID;
	static bool g_bLook;

	Obj* m_owner;
	Vector2 m_offsetPos;
	Vector2 m_finalPos;
	Vector2 m_scale;

	UINT m_ID;
	int m_collisionCount;

public :
	void SetOffsetPos(Vector2 _pos) { m_offsetPos = _pos; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }
	static void SetLook(bool _look) { g_bLook = _look; }

	Vector2 GetFinalPos() { return m_finalPos; }
	Vector2 GetScale() { return m_scale; }
	UINT GetID() { return m_ID; }
	static bool GetLook() { return g_bLook; }

	void update();
	void lateUpdate();
	void render(HDC _dc);

	void OnColliderEnter(Collider* _other);
	void OnCollider(Collider* _other);
	void OnColliderExit(Collider* _other);

	Collider& operator =(const Collider& _other) = delete;

	CLONE(Collider)

public :
	Collider();
	~Collider();

	friend class Obj;
	friend class Player;

private :
	Collider(const Collider& _other);
};

