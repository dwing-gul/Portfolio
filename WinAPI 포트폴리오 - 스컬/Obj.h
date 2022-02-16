#pragma once

class Texture;
class Animator;
class Collider;

class Obj
{
private :
	wstring m_name;
	Vector2  m_pos;
	Vector2  m_scale;
	Texture* m_texture;
	GROUP_TYPE  m_type;

	Animator* m_animator;
	Collider* m_collider;

	bool m_bDead;
	bool m_bForAnimAdjust;
	bool m_bItem;

public :
	void SetName(const wstring& _name) { m_name = _name; }
	void SetPos(Vector2 _pos) { m_pos = _pos; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }
	void SetTexture(Texture* _tex) { m_texture = _tex; }
	void SetAnimAdjust(bool _animAdjust) { m_bForAnimAdjust = _animAdjust; }
	void SetType(GROUP_TYPE _type) { m_type = _type; }
	void SetItem(bool _item) { m_bItem = _item; }

	const wstring& GetName() { return m_name; }
	Vector2 GetPos() { return m_pos; }
	Vector2 GetScale() { return m_scale; }
	Texture* GetTexture() { return m_texture; }
	GROUP_TYPE GetType() { return m_type; }
	Animator* GetAnimator() { return m_animator; }
	Collider* GetCollider() { return m_collider; }
	bool IsDead() { return m_bDead; }
	bool IsForAnimAdjust() { return m_bForAnimAdjust; }
	bool IsItem() { return m_bItem; }

	void CreateCollider();
	void DeleteCollider();
	void CreateAnimator();
	void DeleteAnimator();

	void CreateTexture(const wstring& key, const wstring& relativePath);

	virtual void init() {}
	virtual void update() = 0;
	virtual void lateUpdate();
	virtual void render(HDC _dc);

	virtual void OnColliderEnter(Obj* _other) {}
	virtual void OnCollider(Obj* _other) {}
	virtual void OnColliderExit(Obj* _other) {}

	virtual Obj* Clone() = 0;

	virtual void Load(FILE* _file);

public :
	Obj();
	virtual ~Obj();

	friend class Scene;
	friend class EventManager;
	friend class Background;

protected :
	Obj(const Obj& _origin);
};

