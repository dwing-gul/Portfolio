#include "pch.h"
#include "CNaviMesh.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CTimeManager.h"
#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CUIManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CCamera.h"
#include "CUIManager.h"
#include "CKeyManager.h"

CNaviMesh::CNaviMesh() :
	CComponent(COMPONENT_TYPE::NAVIMESH),
	m_pointPos(Vec3(0.f, 0.f, 0.f)),
	m_bRotRenew(false),
	m_bRenew(false),
	m_bMove(false),
	m_bRot(true),
	m_bWireFrame(false),
	m_rotAngle(0.f),
	m_rotTime(0.f)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh& _origin) :
	CComponent(COMPONENT_TYPE::NAVIMESH),
	m_pointPos(Vec3(0.f, 0.f, 0.f)),
	m_bRotRenew(false),
	m_bRenew(false),
	m_bMove(false),
	m_bRot(true),
	m_bWireFrame(false),
	m_rotAngle(0.f),
	m_rotTime(0.f)
{
	for (size_t i = 0; i < _origin.m_vecNaviMeshNode.size(); ++i)
	{
		CNaviTileNode* node = _origin.m_vecNaviMeshNode[i]->Clone();
		m_vecNaviMeshNode.push_back(node);
	}
}

CNaviMesh::~CNaviMesh()
{
	Safe_Delete_Vec(m_vecNaviMeshNode);
}

void CNaviMesh::start()
{
	/*Ptr<CMesh> mesh = MeshRender()->GetMesh();
	if (MeshRender() && nullptr != mesh)
	{
		tVertex* vertex = (tVertex*)mesh->GetVertexSysMem();
		int vertexCount = mesh->GetVertexCount();
		const vector<tIndexInfo>& indexInfo = mesh->GetIndexInfo();
		vector<tVertex> vecVertex;
		vector<UINT> vecIndex;

		for (int i = 0; i < vertexCount; ++i)
		{
			tVertex point = vertex[i];
			vecVertex.push_back(point);
		}

		for (int i = 0; i < indexInfo.size(); ++i)
		{
			int indexCount = indexInfo[i].indexCount;
			UINT* indexArr = (UINT*)indexInfo[i].indexSysMem;

			for (int j = 0; j < indexCount; ++j)
			{
				UINT index = indexArr[j];
				vecIndex.push_back(index);
			}
		}

		for (size_t i = 0; i < vecIndex.size(); i += 3)
		{
			CNaviTileNode* tile = new CNaviTileNode;
			tile->m_vertex[0] = vecVertex[vecIndex[i]];
			tile->m_vertex[1] = vecVertex[vecIndex[i + 1]];
			tile->m_vertex[2] = vecVertex[vecIndex[i + 2]];

			m_vecNaviMeshNode.push_back(tile);
		}

		for (size_t i = 0; i < m_vecNaviMeshNode.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < m_vecNaviMeshNode.size(); ++j)
			{
				if (m_vecNaviMeshNode[i]->FindAroundTile(m_vecNaviMeshNode[j]))
				{
					m_vecNaviMeshNode[i]->SetAroundTile(m_vecNaviMeshNode[j]->m_ID);
				}
			}
		}
	}*/
}

void CNaviMesh::update()
{
	
}

void CNaviMesh::lateUpdate()
{
}

void CNaviMesh::finalUpdate()
{
	if (m_bWireFrame)
	{
		MeshRender()->GetSharedMaterial()->GetShader()->SetRSType(RASTERIZER_TYPE::WIREFRAME);
	}
	else
	{
		MeshRender()->GetSharedMaterial()->GetShader()->SetRSType(RASTERIZER_TYPE::CULL_BACK);
	}

	m_bRotRenew = false;
	m_bRenew = false;
	
	if (SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode() && CKeyManager::GetInst()->GetMainWinMouseFocus() && CUIManager::GetInst()->GetUI() == nullptr && IsMouseClicked())
	{
		CCamera* cam = CRenderManager::GetInst()->GetMainCam();

		if (nullptr == cam)
			return;

		tRay ray = cam->GetRay();
		Matrix worldMatrix = Transform()->GetWorldMatrix();

		for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
		{
			float result = 0.f;
			Vec3 v0 = m_vecNaviMeshNode[i]->GetTileVertex()[0].pos;
			Vec3 v1 = m_vecNaviMeshNode[i]->GetTileVertex()[1].pos;
			Vec3 v2 = m_vecNaviMeshNode[i]->GetTileVertex()[2].pos;
			Vec3 vertices[3] = { v0, v1, v2 };
			Vec3 crossPoint = Vec3(0.f, 0.f, 0.f);

			// ���콺 ���̿� Ÿ���� �浹 ���� ��
			if (IntersectRay(vertices, ray.start, ray.dir, crossPoint, result))
			{
				m_bMove = true;
				m_bRenew = true;

				if (!isnan(crossPoint.x))
					m_pointPos = crossPoint;
			}
		}
	}

	Reset();
}

Vec3 CNaviMesh::GetMouseClickPos(CCamera* _cam)
{
	Vec3 mousePos = Vec3();

	CCamera* cam = nullptr;

	if ((SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode() || SCENE_MODE::PAUSE == CSceneManager::GetInst()->GetSceneMode())
		&& CKeyManager::GetInst()->GetMainWinMouseFocus() && CUIManager::GetInst()->GetUI() == nullptr && IsMouseClicked())
	{

		if (nullptr == _cam)
			cam = CRenderManager::GetInst()->GetMainCam();
		else
			cam = _cam;

		if (nullptr == cam)
			return mousePos;

		tRay ray = cam->GetRay();
		
		for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
		{
			float result = 0.f;
			Vec3 v0 = m_vecNaviMeshNode[i]->GetTileVertex()[0].pos;
			Vec3 v1 = m_vecNaviMeshNode[i]->GetTileVertex()[1].pos;
			Vec3 v2 = m_vecNaviMeshNode[i]->GetTileVertex()[2].pos;
			Vec3 vertices[3] = { v0, v1, v2 };
			Vec3 crossPoint = Vec3(0.f, 0.f, 0.f);

			// ���콺 ���̿� Ÿ���� �浹 ���� ��
			if (IntersectRay(vertices, ray.start, ray.dir, crossPoint, result))
			{
				if (!isnan(crossPoint.x))
					mousePos = crossPoint;
			}
		}
	}

	return mousePos;
}

bool CNaviMesh::IsAboveTile(CGameObject* _obj,  Vec3 _dir, float _dist)
{
	//--------------------------------------------------------------
	// 1. �ݺ����� ������ � Ÿ�Ͽ� ������Ʈ�� �ִ��� Ȯ�� 
	//		-> � Ÿ�� ������ ���ٸ� false ��ȯ
	// 2. ������Ʈ�� Ÿ�� ���� ���� �� ������Ʈ�� ������ ���⿡ �� �� �ִ� Ÿ���� �ִ°�?
	//		-> ���� ��ġ�� ������ ������ ��ġ�� ����Ͽ� �ش� ��ġ�� Ÿ���� �ִ��� �Ǵ�
	//		-> ���� ��ġ���� true�� ������ dir�� 0�� �ƴ϶�� ������ ������ ��ġ���� Ÿ�� �˻�
	//		-> ������ ���⿡ Ÿ���� ������ false
	//		-> �� ���� �Ұ��� ��� true�� ������ true ��ȯ
	//
	// ================================================================
	//  ������� ��ư��� �κп��� �ַ� false�� �����µ� �̴� Ÿ���� ũ�Ⱑ 
	//	�ʹ� �۾Ƽ� ����� �������̶�� ����
	//--------------------------------------------------------------
	Matrix worldMatrix = Transform()->GetWorldMatrix();
	const tRay& ray = _obj->GetRay();

	Vec3 start = ray.start + _dir * _dist;

	bool check_origin = false;
	bool check_new = false;

	// �� Ÿ�� ���� ĳ���Ͱ� ���� �ִ��� Ȯ��
	for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
	{
		if(nullptr != m_vecNaviMeshNode[i]->m_aboveObject)
			m_vecNaviMeshNode[i]->m_aboveObject = nullptr;

		float dist = 0.f;
		Vec3 v0 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[0].pos, worldMatrix);
		Vec3 v1 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[1].pos, worldMatrix);
		Vec3 v2 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[2].pos, worldMatrix);
	
		DirectX::TriangleTests::Intersects(ray.start, ray.dir, v0, v1, v2, dist);
		
		// �Ÿ� ���� 0�� �ƴ϶�� ĳ���ʹ� �ش� Ÿ�� ���� �ִ�.
		if (0.f != dist)
		{
			check_origin = true;
			break;
		}
		else
		{
			if(_obj == m_vecNaviMeshNode[i]->m_aboveObject)
				m_vecNaviMeshNode[i]->m_aboveObject = nullptr;

			if(nullptr == m_vecNaviMeshNode[i]->m_aboveObject)
				m_vecNaviMeshNode[i]->m_bMove = true;
		}
	}

	if (_dir != Vec3(0.f, 0.f, 0.f))
	{
		for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
		{
			if (nullptr != m_vecNaviMeshNode[i]->m_aboveObject)
				m_vecNaviMeshNode[i]->m_aboveObject = nullptr;

			float dist = 0.f;
			Vec3 v0 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[0].pos, worldMatrix);
			Vec3 v1 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[1].pos, worldMatrix);
			Vec3 v2 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[2].pos, worldMatrix);

			DirectX::TriangleTests::Intersects(start, ray.dir, v0, v1, v2, dist);

			if (nullptr == m_vecNaviMeshNode[i]->m_aboveObject)
				m_vecNaviMeshNode[i]->m_bMove = true;

			// �Ÿ� ���� 0�� �ƴ϶�� ĳ���ʹ� �ش� Ÿ�� ���� �ִ�.
			if (0.f != dist && m_vecNaviMeshNode[i]->m_bMove && (_obj == m_vecNaviMeshNode[i]->m_aboveObject || nullptr == m_vecNaviMeshNode[i]->m_aboveObject))
			{
				check_new = true;
				break;
			}
			else
			{
				continue;
			}
		}

		if (check_new || check_origin)
			return true;
		
		if (!check_new && check_origin)
			return false;
	}
	else
	{
		if (check_origin)
			return true;
	}

	return false;
}

CNaviTileNode* CNaviMesh::GetCurNode(CGameObject* _obj)
{
	Matrix worldMatrix = Transform()->GetWorldMatrix();
	const tRay& ray = _obj->GetRay();

	// �� Ÿ�� ���� ĳ���Ͱ� ���� �ִ��� Ȯ��
	for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
	{
		float dist = 0;
		Vec3 v0 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[0].pos, worldMatrix);
		Vec3 v1 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[1].pos, worldMatrix);
		Vec3 v2 = XMVector3TransformCoord(m_vecNaviMeshNode[i]->GetTileVertex()[2].pos, worldMatrix);

		DirectX::TriangleTests::Intersects(ray.start, ray.dir, v0, v1, v2, dist);

		// �Ÿ� ���� 0�� �ƴ϶�� ĳ���ʹ� �ش� Ÿ�� ���� �ִ�.
		if (0.f != dist)
		{
			return m_vecNaviMeshNode[i];
		}
	}

	return nullptr;
}

void CNaviMesh::FindPath(CNaviTileNode* _curNode, CGameObject* _character, Vec3 _pointPos, Matrix _worldMatrix, list<Vec3>& _waypoints)
{
	if (nullptr == _curNode)
	{
		return;
	}

	CNaviTileNode* curNode = _curNode;
	priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare>& openList = _character->GetOpenList();

	if (!openList.empty())
	{
		while (!openList.empty())
		{
			openList.pop();
		}
	}

	openList.push(curNode);

	Reset();

	Vec3 pos = _character->Transform()->GetLocalPos();
	Vec3 check = Vec3(_pointPos.x, 0.f, _pointPos.z);

	int count = 0;

	Vec3 dest = Vec3(_pointPos.x, pos.y, _pointPos.z);
	float yPos = pos.y;

	if (Vec3::Distance(pos, dest) < 10.f)
		return;

	list<Vec3> queuePos;
	queuePos.push_back(pos);

	while (!openList.empty() && count < 10)
	{
		++count;

		// ���� ����� �ֺ� ��带 OpenList �� �ִ´�.
		const vector<int>& vecAroundID = curNode->GetAroundTile();
		vector<CNaviTileNode*> vecAroundTile;

		for (size_t i = 0; i < vecAroundID.size(); ++i)
		{
			vecAroundTile.push_back(FindAroundTileInfo(vecAroundID[i]));
		}

		for (size_t i = 0; i < vecAroundTile.size(); ++i)
			curNode->AddOpenList(openList, vecAroundTile[i], curNode, _worldMatrix, _pointPos);

		Vec3 v0 = XMVector3TransformCoord(curNode->GetTileVertex()[0].pos, _worldMatrix);
		Vec3 v1 = XMVector3TransformCoord(curNode->GetTileVertex()[1].pos, _worldMatrix);
		Vec3 v2 = XMVector3TransformCoord(curNode->GetTileVertex()[2].pos, _worldMatrix);

		Vec3 nodeCenterPos = (v0 + v1 + v2) / 3.f;

		pos = nodeCenterPos;
		pos.y = yPos;

		float dist = 0.f;
		DirectX::TriangleTests::Intersects(check, Vec3(0.f, 1.f, 0.f), v0, v1, v2, dist);

		if (0.f != dist)
		{
			pos = dest;
		}

		queuePos.push_back(pos);

		// 2. Open List ���� ���� ȿ���� ���� ��带 ������.
		//  - �ش� ���� ClosedList �� �ִ´�.
		curNode = openList.top();
		curNode->m_bClosed = true;

		openList.pop();

		// �������� ������ ��� Astar ����
		if (Vec3::Distance(pos, dest) < 10.f)
		{
			break;
		}
	}

	_waypoints = queuePos;
}

void CNaviMesh::Reset()
{
	for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
	{
		m_vecNaviMeshNode[i]->m_bClosed = false;
		m_vecNaviMeshNode[i]->m_bOpen = false;
	}
}

CNaviTileNode* CNaviMesh::FindAroundTileInfo(int _ID)
{
	for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
	{
		if (_ID == m_vecNaviMeshNode[i]->m_ID)
		{
			return m_vecNaviMeshNode[i];
		}
	}

	return nullptr;
}

void CNaviMesh::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	UINT vertexCount = (UINT)m_vecNaviMeshNode.size();
	SaveToFile(&vertexCount, _file);
	for (size_t i = 0; i < m_vecNaviMeshNode.size(); ++i)
	{
		m_vecNaviMeshNode[i]->SaveToScene(_file);
	}
}

void CNaviMesh::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	UINT vertexCount = 0;
	LoadFromFile(&vertexCount, _file);
	for (UINT i = 0; i < vertexCount; ++i)
	{
		CNaviTileNode* tileNode = new CNaviTileNode;
		tileNode->LoadFromScene(_file);

		m_vecNaviMeshNode.push_back(tileNode);
	}
}
