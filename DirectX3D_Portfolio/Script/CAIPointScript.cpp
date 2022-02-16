#include "pch.h"
#include "CAIPointScript.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"

CAIPointScript::CAIPointScript() :
	CScript((UINT)SCRIPT_TYPE::AIPOINTSCRIPT),
	m_owner(nullptr),
	m_bArrive(false),
	m_bNext(false),
	m_curPointID(0),
	m_prevPointID(0)
{
}

CAIPointScript::~CAIPointScript()
{
}

void CAIPointScript::AddPointID()
{
	m_prevPointID = m_curPointID;
	++m_curPointID;

	if ((int)m_vecPoint.size() <= m_curPointID)
		m_curPointID = 0;
}

void CAIPointScript::SetMovePos()
{
	AddPointID();

	m_bNext = true;
}

void CAIPointScript::SetPointAction()
{
	// 도착한 경우 next 초기화
	m_bNext = false;

	// 플레이어가 해당 point의 도착했을 경우
	switch (m_vecPoint[m_curPointID].m_pointType)
	{
	case POINT_TYPE::MOVE:
	{
		SetMovePos();
	}
	break;
	case POINT_TYPE::LOOTING:
	{
		const vector<CGameObject*>& vecItemSpot = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::INTERACTION, true);
		float distance = 500.f;

		CGameObject* lootingTarget = nullptr;
		for (int i = 0; i < vecItemSpot.size(); ++i)
		{
			float targetDistance = Vec3::Distance(m_vecPoint[m_curPointID].m_pointPos.XZ(), vecItemSpot[i]->Transform()->GetWorldPos().XZ());
			if (distance > targetDistance)
			{
				lootingTarget = vecItemSpot[i];
				distance = targetDistance;
			}
		}
		((CPlayerScript*)m_owner->GetScripts()[0])->SetAILootTarget(lootingTarget);
	}
	break;
	case POINT_TYPE::HUNTING:
	{
		const vector<CGameObject*>& vecMonster = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);

		Vec3 pos = m_vecPoint[m_curPointID].m_pointPos.XZ();
		for (int i = 0; i < (int)vecMonster.size(); ++i)
		{
			if (!vecMonster[i]->IsOn() || ((CMonsterScript*)vecMonster[i]->GetScripts()[0])->IsCharacterDead())
				continue;

			//처음 가장 가까운 몬스터를 죽이고 그 다음에 가까운 몬스터를 죽이러 가야하기 때문에 넣는 순서는 상관 없음
			if (500.f >= Vec3::Distance(pos, vecMonster[i]->Transform()->GetLocalPos().XZ()))
			{
				((CPlayerScript*)m_owner->GetScripts()[0])->FindMonster(vecMonster[i]);
			}
		}
	}
	break;
	}
}

int CAIPointScript::GetFindPointID()
{
	// 처음 이동해야할 ID를 찾는 경우, 혹은 플레이어와 전투가 끝난 경우, 플레이어 빙의가 풀렸을 경우
	if (m_vecPoint.empty())
		return -1;

	float distance = 1000000.f;
	int id = -1;
	for (int i = 0; i < (int)m_vecPoint.size(); ++i)
	{
		float objDistance = Vec3::Distance(m_owner->Transform()->GetLocalPos().XZ(), m_vecPoint[i].m_pointPos.XZ());

		if (distance > objDistance)
		{
			distance = objDistance;
			id = m_vecPoint[i].m_pointID;
		}
	}

	m_curPointID = id;

	return id;
}

void CAIPointScript::SaveToScene(FILE* _file)
{
	UINT vecSize = (UINT)m_vecPoint.size();
	SaveToFile(&vecSize, _file);

	for (size_t i = 0; i < m_vecPoint.size(); ++i)
	{
		SaveToFile(&m_vecPoint[i], _file);
	}
}

void CAIPointScript::LoadFromScene(FILE* _file)
{
	int vecSize = 0;
	LoadFromFile(&vecSize, _file);

	for (UINT i = 0; i < (UINT)vecSize; ++i)
	{
		tPointType pointType = {};
		LoadFromFile(&pointType, _file);
		m_vecPoint.push_back(pointType);
	}
}

//--------------------------------------------------------------
// 역할 : ai가 이동할 이동 포인트 구간 -> 구간에 대한 타입을 받아 각 구간 별로 역할을 부여
// 타입 : 이동, 루팅, 사냥
// 
// - 루팅 : ai가 아이템을 루팅하는 구간 이 때 ai가 해당 구간에서 가장 가까운 아이템 루팅 구간을
//			탐색하여 아이템을 획득하고 아이템 조합을 마침 -> ai가 필요한 아이템만을 얻을 수 있도록 해야됨
// 
// - 사냥 : ai가 야생동물을 잡아 경험치를 얻는 구간. 해당 구간 근처에 도착하면 주변의 야생동물을 잡는다. 
//			또한 잡은 야생동물에게서 아이템을 얻도록 해야됨
// 
// - 진행 순서
// 1. ai가 벡터로 해당 포인트를 가지고 있고 순차적으로 포인트를 이동
// 2. ai가 포인트 도달시 포인트의 타입에 맞게 포인트의 역할 수행
//		2-1. 루팅 포인트인 경우 해당 포인트 도착 시에 해당 포인트에서 가장 가까운 아이템 스폰 탐색
//			 아이템 루팅 후 아이템 조합까지 완료 -> 애초에 원하는 아이템 스폰 옆에 있어야 한다.
//			 -> 포인트에서 가까운 아이템 스폰이 2개 이상일 경우 그 중에서 가장 가까운 1개만을 탐색 or 벡터로 받아서 여러 아이템 스폰을 탐색 
//			 -> 다만 둘 중 어느 것으로 하던지 포인트는 아이템 스폰 가까이에 있어야하며 기존의 ai가 자체적으로 탐색하던 것을 포인트가 탐색하도록 해야됨
//			 -> 그리고 ai의 아이템 스폰 상호작용 범위를 줄이는 것이 좋겠음
//		2-2. 사냥 포인트인 경우 해당 포인트에서 일정 거리만큼 가까워지면 캐릭터 기준 주변의 몬스터 탐색.
//			 -> 몬스터 사냥 시작. 해당 몬스터 죽으면 다른 몬스터 탐색 반복.
//			 -> 해당 포인트의 모든 야생동물이 죽었다면 남아있는 시체에 한하여 루팅 시작
//			 -> 루팅 완료 후 다른 포인트로 이동
// 3. 벡터에 등록된 마지막 포인트까지 도달 하였다면 다시 거꾸로 돌면서 진행
// 4. 중간에 조건이 충족하여 플레이어와 전투를 하는 경우
//		-> 플레이어와 전투 후 현재 위치에서 가장 가까운 포인트 탐색 -> 해당 포인트로 이동 후 다시 1번 부터 진행
// 5. 중간에 플레이어가 ai로 바뀌는 경우
//		-> 현 위치에서 가장 가까운 포인트 탐색 -> 해당 포인트로 이동 후 1번 부터 진행.
// 
// - 각 포인트에서 완료에 관한 bool 값을 조절하여 해당 포인트에서 할일을 모두 마친것을 알렸을 때 다음 포인트로 진행하도록 유도.
// - 다른 포인트 출발 시 그 전 포인트의 bool 값은 초기화
// 
// - owner의 위치와 포인트 사이의 거리를 측정하여 일정 거리(이건 타입별로 정해줘야됨)안으로 들어오면 도착 확인 후 해당 포인트에서의 역할 진행
// - 다음 포인트로 넘어갈 때는 next 관련 bool 값을 조사하여 해당 부분이 true이면 출발 지점에서는 역할 진행 하지 않음
//--------------------------------------------------------------