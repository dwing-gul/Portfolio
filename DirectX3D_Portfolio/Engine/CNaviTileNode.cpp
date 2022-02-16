#include "pch.h"
#include "CNaviTileNode.h"

int g_nextTileID = 0;

CNaviTileNode::CNaviTileNode() :
	m_ID(g_nextTileID++),
	m_aboveObject(nullptr),
	m_prevNode(nullptr),
	m_fromParent(0.f),
	m_toDest(0.f),
	m_final(0.f),
	m_bMove(true),
	m_bOpen(false),
	m_bClosed(false)
{
}

CNaviTileNode::CNaviTileNode(const CNaviTileNode& _origin) :
	m_ID(_origin.m_ID),
	m_aboveObject(nullptr),
	m_prevNode(nullptr),
	m_fromParent(0.f),
	m_toDest(0.f),
	m_final(0.f),
	m_bMove(true),
	m_bOpen(false),
	m_bClosed(false)
{
	for (size_t i = 0; i < _origin.m_aroundTile.size(); ++i)
	{
		int clone = _origin.m_aroundTile[i];

		m_aroundTile.push_back(clone);
	}

	for (int i = 0; i < 3; ++i)
	{
		m_vertex[i] = _origin.m_vertex[i];
	}
}

CNaviTileNode::~CNaviTileNode()
{
}

void CNaviTileNode::Rebuild(priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare>& _queue)
{
	priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare> tempQueue;

	while (!_queue.empty())
	{
		tempQueue.push(_queue.top());
		_queue.pop();
	}

	_queue.swap(tempQueue);
}

void CNaviTileNode::CalculateCost(CNaviTileNode* _curNode, CNaviTileNode* _origin, Matrix _worldMatrix, Vec3 _pointPos)
{
	// 타일을 이루는 정점의 위치
	Vec3 prevVert0Pos = XMVector3TransformCoord(_origin->GetTileVertex()[0].pos, _worldMatrix);
	Vec3 prevVert1Pos = XMVector3TransformCoord(_origin->GetTileVertex()[1].pos, _worldMatrix);
	Vec3 prevVert2Pos = XMVector3TransformCoord(_origin->GetTileVertex()[2].pos, _worldMatrix);

	// 타일을 이루는 정점의 위치
	Vec3 vert0Pos = XMVector3TransformCoord(_curNode->GetTileVertex()[0].pos, _worldMatrix);
	Vec3 vert1Pos = XMVector3TransformCoord(_curNode->GetTileVertex()[1].pos, _worldMatrix);
	Vec3 vert2Pos = XMVector3TransformCoord(_curNode->GetTileVertex()[2].pos, _worldMatrix);

	Vec3 prevCenterPos = (prevVert0Pos + prevVert1Pos + prevVert2Pos) / 3.f;
	Vec3 curCenterPos = (vert0Pos + vert1Pos + vert2Pos) / 3.f;

	_curNode->m_prevNode = _origin;
	_curNode->m_fromParent = Vec3::Distance(prevCenterPos, curCenterPos);
	_curNode->m_toDest = Vec3::Distance(curCenterPos, _pointPos);
	_curNode->m_final = _curNode->m_fromParent + _curNode->m_toDest;
}

void CNaviTileNode::AddOpenList(priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare>& _queue, CNaviTileNode* _aroundTile,
	CNaviTileNode* _origin, Matrix _worldMatrix, Vec3 _pointPos)
{
	// 현재 지점에서 갈 수 있는 곳을 OpenList 에 넣는다.
	// 노드 범위를 벗어난 경우
	if (!_aroundTile->m_bMove)
		return;

	// 해당 길이 Clost List 에 있는 경우, Open List 에 넣지 않는다.
	if (_aroundTile->m_bClosed)
		return;

	// Open List 에 비용을 계산해서 넣는다.
	if (false == _aroundTile->m_bOpen)
	{
		CalculateCost(_aroundTile, _origin, _worldMatrix, _pointPos);
		// Open List 에 넣는다.
		_aroundTile->m_bOpen = true;
		_queue.push(_aroundTile);
	}
	else // 이미 OpenList 에 있는 경우,
	{
		//비용을 계산해서 더 효율이 좋은 것으로 대체한다.
		float finalDist = _aroundTile->m_final;
		CalculateCost(_aroundTile, _origin, _worldMatrix, _pointPos);

		if (finalDist > _aroundTile->m_final)
		{
			// 오픈리스트(우선순위큐) 재설정
			Rebuild(_queue);
		}
	}
}

void CNaviTileNode::SaveToScene(FILE* _file)
{
	for (int i = 0; i < 3; ++i)
	{
		SaveToFile(&m_vertex[i], _file);
	}

	SaveToFile(&m_ID, _file);

	UINT aroundTileCount = (UINT)m_aroundTile.size();
	SaveToFile(&aroundTileCount, _file);

	for (size_t i = 0; i < m_aroundTile.size(); ++i)
	{
		SaveToFile(&m_aroundTile[i], _file);
	}
}

void CNaviTileNode::LoadFromScene(FILE* _file)
{
	for (int i = 0; i < 3; ++i)
	{
		LoadFromFile(&m_vertex[i], _file);
	}

	LoadFromFile(&m_ID, _file);

	UINT aroundTileCount = 0;
	LoadFromFile(&aroundTileCount, _file);

	for (UINT i = 0; i < aroundTileCount; ++i)
	{
		int aroundTileID = -1;
		LoadFromFile(&aroundTileID, _file);

		if(-1 == aroundTileID)
			assert(nullptr);

		m_aroundTile.push_back(aroundTileID);
	}
}

bool CNaviTileNode::FindAroundTile(CNaviTileNode* _target)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (m_vertex[i].pos == _target->m_vertex[j].pos)
			{
				return true;
			}
		}
	}
	return false;
}

bool CNaviTileNode::FindAroundTile_Other(CNaviTileNode* _target, Matrix _world, Matrix _otherWorld)
{
	Vec3 vert0Pos = m_vertex[0].pos;
	Vec3 vert1Pos = m_vertex[1].pos;
	Vec3 vert2Pos = m_vertex[2].pos;

	Vec3 otherVert0Pos = _target->m_vertex[0].pos;
	Vec3 otherVert1Pos = _target->m_vertex[1].pos;
	Vec3 otherVert2Pos = _target->m_vertex[2].pos;

	// 정점의 위치를 월드에서의 위치로 변환
	Matrix v0Translation = XMMatrixTranslation(vert0Pos.x, vert0Pos.y, vert0Pos.z);
	Matrix v1Translation = XMMatrixTranslation(vert1Pos.x, vert1Pos.y, vert1Pos.z);
	Matrix v2Translation = XMMatrixTranslation(vert2Pos.x, vert2Pos.y, vert2Pos.z);

	Matrix otherV0Translation = XMMatrixTranslation(otherVert0Pos.x, otherVert0Pos.y, otherVert0Pos.z);
	Matrix otherV1Translation = XMMatrixTranslation(otherVert1Pos.x, otherVert1Pos.y, otherVert1Pos.z);
	Matrix otherV2Translation = XMMatrixTranslation(otherVert2Pos.x, otherVert2Pos.y, otherVert2Pos.z);

	Vec3 vertex[3];
	vertex[0] = (v0Translation * _world).Translation();
	vertex[1] = (v1Translation * _world).Translation();
	vertex[2] = (v2Translation * _world).Translation();

	Vec3 otherVertex[3];
	otherVertex[0] = (otherV0Translation * _otherWorld).Translation();
	otherVertex[1] = (otherV1Translation * _otherWorld).Translation();
	otherVertex[2] = (otherV2Translation * _otherWorld).Translation();

	for (int i = 0; i < 3; ++i)
	{
		
		for (int j = 0; j < 3; ++j)
		{
			if (vertex[i] == otherVertex[j])
			{
				return true;
			}
		}
	}

	return false;
}
