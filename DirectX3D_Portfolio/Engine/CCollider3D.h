#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CCollider3D :
	public CComponent
{
private:
	Vec3				m_offsetPos;
	Vec3				m_offsetScale;
	Vec3				m_worldOffsetScale;
	Matrix				m_colliderWorldMatrix;

	Ptr<CMesh>			m_mesh;
	Ptr<CMaterial>		m_material;

	CCollider3D*		m_target;

	vector<CScript*>	m_vecScript;

	int					m_collisionCount;
	bool				m_bCollision;

	bool				m_bAdjust; // 충돌체 위치 조정을 위한 변수
	bool				m_bSphere; // 충돌체 모양을 구 모양을 하려고 할 때
	bool				m_bOn; // 충돌체 온/오프를 위한 변수(충돌체 있음/ 없음)

public:
	Vec3 GetOffsetPos() { return m_offsetPos; }
	Vec3 GetOffsetScale() { return m_offsetScale; }
	Vec3 GetWorldOffSetScale() { return m_worldOffsetScale; }
	Matrix GetWorldMatrix() { return m_colliderWorldMatrix; }
	CCollider3D* GetTarget() { return m_target; }
	Ptr<CMesh> GetMesh() { return m_mesh; }

	bool IsCollision() { return m_bCollision; }
	bool IsSphere() { return m_bSphere; }
	bool IsOn() { return m_bOn; }

	void SetOffsetPos(Vec3 _pos) { m_offsetPos = _pos; }
	void SetOffsetScale(Vec3 _scale);
	void SetAdjust(bool _bAdjust) { m_bAdjust = _bAdjust; }
	void SetSphere(bool _bSphere);
	void SetOnOff(bool _bOn) { m_bOn = _bOn; }

	void OnColliderEnter(CCollider3D* _other);
	void OnCollider(CCollider3D* _other);
	void OnColliderExit(CCollider3D* _other);

	virtual void update();
	virtual void finalUpdate();
	void render();

	virtual void UpdateData();

	virtual void SaveToScene(FILE* _file);
	virtual void LoadFromScene(FILE* _file);

	CLONE(CCollider3D);

public:
	CCollider3D();
	CCollider3D(const CCollider3D& _origin);
	~CCollider3D();
};
