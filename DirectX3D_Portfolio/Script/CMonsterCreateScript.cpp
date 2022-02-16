#include "pch.h"
#include "CMonsterCreateScript.h"
#include "CMonsterScript.h"

CMonsterCreateScript::CMonsterCreateScript():
	CScript((UINT)SCRIPT_TYPE::MONSTERCREATESCRIPT),
	m_bat(nullptr),
	m_bear(nullptr),
	m_boar(nullptr),
	m_chicken(nullptr),
	m_dog(nullptr),
	m_wolf(nullptr),
	m_monsterType(MONSTER_TYPE::END)
{
}

CMonsterCreateScript::~CMonsterCreateScript()
{
}

void CMonsterCreateScript::MonsterCreate()
{
	// 테스트 함수
	// 해당 위치에 제대로 생성이 되는지 확인하기 위한 용도
	CGameObject* obj;
	Vec3 Pos = Object()->Transform()->GetLocalPos();
	Pos.y = 0.f;

	switch (m_monsterType)
	{
	case MONSTER_TYPE::MONSTER_BAT:
		obj = m_bat->Instantiate();
		Pos.y = 150.f;
		break;
	case MONSTER_TYPE::MONSTER_BEAR:
		//obj = m_bear->Instantiate();
		return;
		break;
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
	((CMonsterScript*)obj->GetScripts()[0])->SetStartPos(Pos);
	CScript::CreateObject(obj);
}

void CMonsterCreateScript::start()
{
	m_bat = CResourceManager::GetInst()->Load<CPrefab>(L"Bat", L"prefab\\Bat_01.prefab").Get();
	m_bear = CResourceManager::GetInst()->Load<CPrefab>(L"Bear", L"prefab\\Bear_01.prefab").Get();
	m_boar = CResourceManager::GetInst()->Load<CPrefab>(L"Boar", L"prefab\\Boar_01.prefab").Get();
	m_chicken = CResourceManager::GetInst()->Load<CPrefab>(L"Chicken", L"prefab\\Chicken_01.prefab").Get();
	m_dog = CResourceManager::GetInst()->Load<CPrefab>(L"Dog", L"prefab\\Dog_01.prefab").Get();
	m_wolf = CResourceManager::GetInst()->Load<CPrefab>(L"Wolf", L"prefab\\Wolf_01.prefab").Get();
}

void CMonsterCreateScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_monsterType, _file);
}

void CMonsterCreateScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_monsterType, _file);
}
