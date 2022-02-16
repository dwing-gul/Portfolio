#include "pch.h"
#include "CMonsterManager.h"
#include <Engine\CResourceManager.h>
#include <Engine\CPrefab.h>
#include "CMonsterCreateScript.h"

#define ALLEYMAPLAYER 1

CMonsterManager::CMonsterManager():
	m_bat(nullptr),
	m_bear(nullptr),
	m_boar(nullptr),
	m_chicken(nullptr),
	m_dog(nullptr),
	m_wolf(nullptr),
	m_bRegen(false),
	m_monsterCreatObj(nullptr),
	m_bStart(false),
	m_totalTime(0.f)
{
}

CMonsterManager::~CMonsterManager()
{
}

void CMonsterManager::init()
{
	//해당 init에서 몬스터 불러오기
	m_bat = CResourceManager::GetInst()->Load<CPrefab>(L"Bat", L"prefab\\Bat_01.prefab").Get();
	m_bear = CResourceManager::GetInst()->Load<CPrefab>(L"Bear", L"prefab\\Bear_01.prefab").Get();
	m_boar = CResourceManager::GetInst()->Load<CPrefab>(L"Boar", L"prefab\\Boar_01.prefab").Get();
	m_chicken = CResourceManager::GetInst()->Load<CPrefab>(L"Chicken", L"prefab\\Chicken_01.prefab").Get();
	m_dog = CResourceManager::GetInst()->Load<CPrefab>(L"Dog", L"prefab\\Dog_01.prefab").Get();
	m_wolf = CResourceManager::GetInst()->Load<CPrefab>(L"Wolf", L"prefab\\Wolf_01.prefab").Get();

	m_monsterCreatObj = CResourceManager::GetInst()->Load<CPrefab>(L"AlleyMonsterSpawnPoint", L"prefab\\AlleyMonsterSpawnPoint.prefab").Get();
}

void CMonsterManager::update()
{
	if (!m_bStart && CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
	{
		CreateMonster();
		m_bStart = true;
	}

	if (CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
	{
		for (int i = 0; i < m_vecDeadObj.size(); ++i)
		{
			m_vecDeadObj[i].DeadTime += DELTATIME;

			if (m_vecDeadObj[i].bRender && m_vecDeadObj[i].DeadTime >= 30.f)
			{
				m_vecDeadObj[i].bRender = false;
				m_vecDeadObj[i].MonsterScript->Object()->SetRenderCheck(false);
				m_vecDeadObj[i].MonsterScript->Object()->SetObjOff();
			}

			if (m_vecDeadObj[i].DeadTime >= 180.f)
			{
				// 인게임 시간을 관리하는 오브젝트를 받아와서 해당 시간을 통하여 리젠 되는 몬스터의 레벨을 정해준다.
				float ingameTime = m_totalTime;
				ingameTime /= 60.f;
				m_vecDeadObj[i].MonsterScript->SetMonsterLv((UINT)ingameTime);
				m_vecDeadObj[i].MonsterScript->MonsterRegen();
				m_vecDeadObj[i].bRegen = true;
				m_bRegen = true;
			}
		}

		if (m_bRegen)
		{
			vector<tMonsterDead>::iterator iter = m_vecDeadObj.begin();
			for (; iter != m_vecDeadObj.end();)
			{
				if ((*iter).bRegen == true)
					iter = m_vecDeadObj.erase(iter);
				else
					++iter;
			}
			m_bRegen = false;
		}
	}
	else if (CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::STOP)
	{
		vector<tMonsterDead>::iterator iter = m_vecDeadObj.begin();
		for (; iter != m_vecDeadObj.end();)
		{
			iter->MonsterScript->MonsterRegen();
			iter = m_vecDeadObj.erase(iter);
		}
	}
}

void CMonsterManager::CreateMonster()
{
	//최상위 부모인 빈 오브젝트를 받아옴
	CGameObject* monsterCreatObj = m_monsterCreatObj->Instantiate();
	//해당 자식 오브젝트는 몬스터 타입과 위치 값을 가지고 있는 빈 오브젝트
	vector<CGameObject*> vecmonsterCreatObj = monsterCreatObj->GetChilds();


	for (int i = 0; i < vecmonsterCreatObj.size(); ++i)
	{
		vector<CGameObject*> vecMonsterObj = vecmonsterCreatObj[i]->GetChilds();
		for (int j = 0; j < vecMonsterObj.size(); ++j)
		{
			MONSTER_TYPE eType = ((CMonsterCreateScript*)vecMonsterObj[j]->GetScripts()[0])->GetMonsterType();
			CGameObject* obj = nullptr;
			Vec3 Pos = vecMonsterObj[j]->Transform()->GetLocalPos();
			Pos.y = 0.f;

			switch (eType)
			{
			case MONSTER_TYPE::MONSTER_BAT:
				obj = m_bat->Instantiate();
				Pos.y = 150.f;
				break;
			case MONSTER_TYPE::MONSTER_BEAR:
				continue;
			case MONSTER_TYPE::MONSTER_BOAR:
				obj = m_boar->Instantiate();
				break;
			case MONSTER_TYPE::MONSTER_CHICKEN:
				obj = m_chicken->Instantiate();
				break;
			case MONSTER_TYPE::MONSTER_DOG:
				obj = m_dog->Instantiate();
				break;
			case MONSTER_TYPE::MONSTER_WOLF:
				obj = m_wolf->Instantiate();
				break;
			case MONSTER_TYPE::END:
				return;
			default:
				return;
			}
			obj->Transform()->SetLocalPos(Pos);
			CScript::CreateObject(obj, ALLEYMAPLAYER);
		}
	}

	delete monsterCreatObj;
}

void CMonsterManager::SetMonsterDead(CMonsterScript* _monsterScript)
{
	tMonsterDead tMonster; 
	tMonster.DeadTime = 0.f; 
	tMonster.MonsterScript = _monsterScript;
	tMonster.bRender = true;
	m_vecDeadObj.push_back(tMonster);
}

void CMonsterManager::SetMonsterSetting(MONSTER_TYPE _type)
{
}
