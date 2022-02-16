#pragma once

#include "CMonsterScript.h"

struct tMonsterDead
{
	CMonsterScript* MonsterScript;
	float			DeadTime;
	bool			bRender;
	bool			bRegen;
};

class CObjectScript;

class CMonsterManager :
	public CSingleton<CMonsterManager>
{
	SINGLE(CMonsterManager);
private:
	CPrefab* m_bat;
	CPrefab* m_bear;
	CPrefab* m_boar;
	CPrefab* m_chicken;
	CPrefab* m_dog;
	CPrefab* m_wolf;
	CPrefab* m_monsterCreatObj;
	vector<vector<CGameObject*>>	m_vecMonsterCreatMgr;

	float					m_totalTime;
	bool					m_bRegen;
	bool					m_bStart;
	vector<tMonsterDead>	m_vecDeadObj;

public:
	void init();
	void update();

	void SetTotalTime(float _time) { m_totalTime = _time; }
	void CreateMonster();
	void SetMonsterDead(CMonsterScript* _monsterScript);
	void SetMonsterSetting(MONSTER_TYPE _type);
};

