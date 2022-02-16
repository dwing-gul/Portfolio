#pragma once
#include "CComponent.h"
#include "CMaterial.h"
#include "CMesh.h"

class CCollider2D :
	public CComponent
{
private:
	Vec2 m_offsetPos;
	Vec2 m_offsetScale;
	Matrix m_colliderWorldMatrix;

	Ptr<CMesh> m_mesh;
	Ptr<CMaterial> m_material;

	int m_collisionCount;
	bool          m_bCollision;
	int           m_hovered;

public:
	Vec2 GetOffsetPos() { return m_offsetPos; }
	Vec2 GetOffsetScale() { return m_offsetScale; }
	Matrix GetWorldMatrix() { return m_colliderWorldMatrix; }
	bool IsCollision() { return m_bCollision; }

	void SetOffsetPos(Vec2 _pos) { m_offsetPos = _pos; }
	void SetOffsetScale(Vec2 _scale) { m_offsetScale = _scale; }

	void OnColliderEnter(CCollider2D* _other);
	void OnCollider(CCollider2D* _other);
	void OnColliderExit(CCollider2D* _other);

	virtual void update();
	virtual void finalUpdate();
	void render();

	virtual void UpdateData();

	virtual void SaveToScene(FILE* _file);
	virtual void LoadFromScene(FILE* _file);

	bool IsObjectHovered();

	CLONE(CCollider2D);

public:
	CCollider2D();
	CCollider2D(const CCollider2D& _origin);
	~CCollider2D();
};


