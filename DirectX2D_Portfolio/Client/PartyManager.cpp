#include "pch.h"
#include "PartyManager.h"
#include "SceneConverter.h"
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/Collider2D.h>
#include <Engine/Animator2D.h>
#include <Engine/Animation2D.h>
#include <Engine/Camera.h>
#include <Engine/Light2D.h>
#include <Engine/KeyManager.h>
#include <Engine/SceneManager.h>
#include <Engine/TimeManager.h>
#include <Script/PlayerScript.h>
#include <Script/MonsterScript.h>
#include <Script/ScriptManager.h>
#include <Script/DoorScript.h>
#include <Script/CameraScript.h>

PartyManager::PartyManager() :
	m_targetDoor(nullptr),
	m_bEnter(false),
	m_bBattle(false),
	m_bChangeScene(false),
	m_bTurn(true),
	m_bNext(false),
	m_bDoorSound(true),
	m_bTown(false),
	m_bFirstTurn(true),
	m_battleSoundChannel(0),
	m_exploreSoundChannel(0),
	m_townSoundChannel(0),
	m_turnCount(1),
	m_startCheck(0)
{
}

PartyManager::~PartyManager()
{
}

void PartyManager::init()
{
	GameObject* dummy = nullptr;

	for (int i = 0; i < 4; ++i)
	{
		m_vectorPlayerParty.push_back(dummy);
	}

	for (int i = 0; i < 4; ++i)
	{
		m_vectorMonsterParty.push_back(dummy);
	}
}

void PartyManager::update()
{
	CheckScene();
	CheckDoor();

	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
	float height = 0.f;

	const vector<Script*> vectorCameraScript = camera->GetScripts();

	for (size_t i = 0; i < vectorCameraScript.size(); ++i)
	{
		if (vectorCameraScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::CAMERASCRIPT)
		{
			height = ((CameraScript*)vectorCameraScript[i])->GetFloor() * 800.f;
		}
	}

	if (!m_bEnter)
	{
		for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
		{
			if (nullptr != m_vectorPlayerParty[i] && m_vectorPlayerParty[i]->IsDead())
			{
				if (m_vectorPlayerParty.size() - 1 == i)
				{
					m_vectorPlayerParty[i] = nullptr;
					continue;
				}

				for (size_t j = i; j < m_vectorPlayerParty.size() - 1; ++j)
				{
					if (nullptr == m_vectorPlayerParty[j] || m_vectorPlayerParty[j]->IsDead())
						continue;

					Vector3 pos = m_vectorPlayerParty[j]->GetTransform()->GetLocalPos();
					if (nullptr != m_vectorPlayerParty[j + 1] && !m_vectorPlayerParty[j + 1]->IsDead())
					{
						m_vectorPlayerParty[j] = m_vectorPlayerParty[j + 1];
						m_vectorPlayerParty[j]->GetTransform()->SetLocalPos(pos);
						m_vectorPlayerParty[j + 1] = nullptr;
					}
				}
			}

			for (size_t k = m_vectorPlayerParty.size() - 1; k > 0; --k)
			{
				if (nullptr == m_vectorPlayerParty[k] || m_vectorPlayerParty[k]->IsDead())
					continue;

				Vector3 pos = m_vectorPlayerParty[k]->GetTransform()->GetLocalPos();
				if (nullptr == m_vectorPlayerParty[k - 1])
				{
					pos.x += 150.f;
					m_vectorPlayerParty[k - 1] = m_vectorMonsterParty[k];

					if (nullptr == m_vectorPlayerParty[k - 1])
						continue;

					m_vectorPlayerParty[k - 1]->GetTransform()->SetLocalPos(pos);
					m_vectorPlayerParty[k] = nullptr;
				}
				else if (nullptr != m_vectorPlayerParty[k - 1] && m_vectorPlayerParty[k - 1]->IsDead())
				{
					pos.x += 150.f;
					m_vectorPlayerParty[k - 1] = m_vectorPlayerParty[k];
					m_vectorPlayerParty[k - 1]->GetTransform()->SetLocalPos(pos);
					m_vectorPlayerParty[k] = nullptr;
				}
			}

			if (nullptr == m_vectorPlayerParty[i] || m_vectorPlayerParty[i]->IsDead())
				continue;

			if (0 != i)
			{
				Vector3 pos = m_vectorPlayerParty[i]->GetTransform()->GetLocalPos();

				if (nullptr != m_vectorPlayerParty[i - 1])
				{
					if (!(((PlayerScript*)m_vectorPlayerParty[i - 1]->GetScripts()[0])->IsMove()))
					{
						pos.x = m_vectorPlayerParty[i - 1]->GetTransform()->GetLocalPos().x - 130.f;
						pos.y = height + 40.f;
						pos.z = 110.f;
						m_vectorPlayerParty[i]->GetTransform()->SetLocalPos(pos);
					}
				}
			}
			else
			{
				if (!(((PlayerScript*)m_vectorPlayerParty[i]->GetScripts()[0])->IsMove()))
				{
					Vector3 pos = m_vectorPlayerParty[i]->GetTransform()->GetLocalPos();
					pos.y = height + 40.f;
					pos.z = 110.f;
					m_vectorPlayerParty[i]->GetTransform()->SetLocalPos(pos);
				}
			}

			const vector<Script*> vectorPlayer = m_vectorPlayerParty[i]->GetScripts();

			if (vectorPlayer.empty())
				continue;

			if (0 == i)
			{
				((PlayerScript*)vectorPlayer[0])->SetNextObj(m_vectorPlayerParty[i + 1]);
				((PlayerScript*)vectorPlayer[0])->SetPrevObj(nullptr);
			}
			else if(0 < i && i < m_vectorPlayerParty.size() - 1)
			{
				((PlayerScript*)vectorPlayer[0])->SetNextObj(m_vectorPlayerParty[i + 1]);
				((PlayerScript*)vectorPlayer[0])->SetPrevObj(m_vectorPlayerParty[i - 1]);
			}
			else
			{
				((PlayerScript*)vectorPlayer[0])->SetNextObj(nullptr);
				((PlayerScript*)vectorPlayer[0])->SetPrevObj(m_vectorPlayerParty[i - 1]);
			}
		}
	}
	else
	{
		if (m_bDoorSound)
		{
			m_bDoorSound = false;
			Ptr<Sound> enter = ResourceManager::GetInst()->Load<Sound>(L"Sound_Door_Open", L"sound\\dungeon\\gen_map_door_open.wav");
			int channel = enter->Play(1);
			enter->SetVolume(0.5f, channel);
		}

		// 문으로 들어가는 연출을 해줘야됨
		EnterTheDoor();
	}

	if (m_bBattle)
	{
		for (size_t i = 0; i < m_vectorMonsterParty.size(); ++i)
		{
			if (nullptr != m_vectorMonsterParty[i] && m_vectorMonsterParty[i]->IsDead())
			{
				if (m_vectorMonsterParty[i]->IsDead())
				{
					m_vectorMonsterParty[i] = nullptr;
					continue;
				}

				for (size_t j = i; j < m_vectorMonsterParty.size() - 1; ++j)
				{
					if (nullptr == m_vectorMonsterParty[j] || m_vectorMonsterParty[j]->IsDead())
						continue;

					Vector3 pos = m_vectorMonsterParty[j]->GetTransform()->GetLocalPos();
					if (nullptr != m_vectorMonsterParty[j + 1] && !m_vectorMonsterParty[j + 1]->IsDead())
					{
						m_vectorMonsterParty[j] = m_vectorMonsterParty[j + 1];
						m_vectorMonsterParty[j]->GetTransform()->SetLocalPos(pos);
						m_vectorMonsterParty[j + 1] = nullptr;
					}
				}
			}
			else if (nullptr != m_vectorMonsterParty[i] && !m_vectorMonsterParty[i]->IsDead())
			{
				for (size_t k = m_vectorMonsterParty.size() - 1; k > 0; --k)
				{
					if (nullptr == m_vectorMonsterParty[k] || m_vectorMonsterParty[k]->IsDead())
						continue;

					Vector3 pos = m_vectorMonsterParty[k]->GetTransform()->GetLocalPos();
					if (nullptr == m_vectorMonsterParty[k - 1])
					{
						pos.x -= 150.f;
						m_vectorMonsterParty[k - 1] = m_vectorMonsterParty[k];
						m_vectorMonsterParty[k - 1]->GetTransform()->SetLocalPos(pos);
						m_vectorMonsterParty[k] = nullptr;
					}
					else if (nullptr != m_vectorMonsterParty[k - 1] && m_vectorMonsterParty[k - 1]->IsDead())
					{
						pos.x -= 150.f;
						m_vectorMonsterParty[k - 1] = m_vectorMonsterParty[k];
						m_vectorMonsterParty[k - 1]->GetTransform()->SetLocalPos(pos);
						m_vectorMonsterParty[k] = nullptr;
					}
				}

				if (nullptr == m_vectorMonsterParty[i])
					continue;

				if (0 != i)
				{
					if (nullptr == m_vectorMonsterParty[i])
						continue;

					Vector3 pos = m_vectorMonsterParty[i]->GetTransform()->GetLocalPos();
					((MonsterScript*)m_vectorMonsterParty[i]->GetScripts()[0])->SetPosition((UINT)i);
					if (nullptr != m_vectorMonsterParty[i - 1] && !m_vectorMonsterParty[i - 1]->IsDead())
					{
						if (!(((MonsterScript*)m_vectorMonsterParty[i - 1]->GetScripts()[0])->IsMove()))
						{
							pos.x = m_vectorMonsterParty[i - 1]->GetTransform()->GetLocalPos().x + 130.f;
							pos.y = height + 40.f;
							pos.z = 110.f;
							m_vectorMonsterParty[i]->GetTransform()->SetLocalPos(pos);
						}
					}
				}
				else
				{
					if (nullptr == m_vectorMonsterParty[i])
						continue;

					if (nullptr == m_vectorPlayerParty[0])
						continue;

					((MonsterScript*)m_vectorMonsterParty[i]->GetScripts()[0])->SetPosition((UINT)i);
					if (!(((MonsterScript*)m_vectorMonsterParty[0]->GetScripts()[0])->IsMove()))
					{
						if (!m_vectorPlayerParty[0]->IsDead())
						{
							Vector3 pos = m_vectorPlayerParty[0]->GetTransform()->GetLocalPos();
							pos.x = m_vectorPlayerParty[0]->GetTransform()->GetLocalPos().x + 300.f;
							pos.y = height + 40.f;
							pos.z = 110.f;
							m_vectorMonsterParty[i]->GetTransform()->SetLocalPos(pos);
						}
					}
				}

				const vector<Script*> vectorMonster = m_vectorMonsterParty[i]->GetScripts();

				if (0 == i)
				{
					((MonsterScript*)vectorMonster[0])->SetNextObj(m_vectorMonsterParty[i + 1]);
					((MonsterScript*)vectorMonster[0])->SetPrevObj(nullptr);
				}
				else if (0 < i && i < m_vectorMonsterParty.size() - 1)
				{
					((MonsterScript*)vectorMonster[0])->SetNextObj(m_vectorMonsterParty[i + 1]);
					((MonsterScript*)vectorMonster[0])->SetPrevObj(m_vectorMonsterParty[i - 1]);
				}
				else
				{
					((MonsterScript*)vectorMonster[0])->SetNextObj(nullptr);
					((MonsterScript*)vectorMonster[0])->SetPrevObj(m_vectorMonsterParty[i - 1]);
				}
			}
		}

		for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
		{
			if (nullptr != m_vectorPlayerParty[i] && !m_vectorPlayerParty[i]->IsDead())
			{
				((PlayerScript*)m_vectorPlayerParty[i]->GetScripts()[0])->SetPosition((UINT)i);
			}
		}

		int check = 0;
		for (size_t i = 0; i < m_vectorMonsterParty.size(); ++i)
		{
			if (nullptr == m_vectorMonsterParty[i])
				++check;
		}

		if (m_vectorMonsterParty.size() == check)
		{
			m_bBattle = false;
			SceneConverter::GetInst()->SetFinish(true);
		}

		if (m_bTurn)
		{
			m_bTurn = false;
			CalOrder();
		}

		BattlePhaseManagement();// 배틀페이즈 종료시 몬스터 파티 초기화
	}

	StopSoundAll();

	if (!m_bBattle)
	{
		ExploreSound();

		if (nullptr == PlayerScript::m_selectPlayer)
		{
			if (nullptr == m_vectorPlayerParty[0])
				return;

			const vector<Script*> vectorPlayer = m_vectorPlayerParty[0]->GetScripts();
			((PlayerScript*)vectorPlayer[0])->SetSelect(true);
			PlayerScript::m_selectPlayer = m_vectorPlayerParty[0];
		}

		for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
		{
			if (nullptr == m_vectorPlayerParty[i])
				continue;

			if (PlayerScript::m_selectPlayer != m_vectorPlayerParty[i])
			{
				const vector<Script*> vectorPlayer = m_vectorPlayerParty[i]->GetScripts();

				if(!vectorPlayer.empty())
					((PlayerScript*)vectorPlayer[0])->SetSelect(false);
			}
		}
	}
}

void PartyManager::AddPlayerParty(UINT _position, GameObject* _player)
{
	if (_position > 3)
		return;

	m_vectorPlayerParty[_position] = _player;
	
	const vector<Script*>& vectorScript = _player->GetScripts();

	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorScript[i]->GetScriptType())
		{
			((PlayerScript*)vectorScript[i])->SetPosition(_position);
		}
	}

	Script::CreateObject(m_vectorPlayerParty[_position], m_vectorPlayerParty[_position]->GetTransform()->GetLocalPos());
}

void PartyManager::ResetPlayerParty()
{
	for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
	{
		m_vectorPlayerParty[i] = nullptr;
	}
}

void PartyManager::AddMonsterParty(UINT _position, GameObject* _monster)
{
	if (_position > 4)
		return;

	m_vectorMonsterParty[_position] = _monster;

	const vector<Script*>& vectorScript = _monster->GetScripts();

	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorScript[i]->GetScriptType())
		{
			((MonsterScript*)vectorScript[i])->SetPosition(_position);
		}
	}

	Script::CreateObject(m_vectorMonsterParty[_position], m_vectorMonsterParty[_position]->GetTransform()->GetLocalPos());
}

void PartyManager::ResetMonsterParty()
{
	for (size_t i = 0; i < m_vectorMonsterParty.size(); ++i)
	{
		m_vectorMonsterParty[i] = nullptr;
	}
}

bool SortOrder(tOrder _left, tOrder _right)
{
	if (_left.speed == _right.speed)
	{
		
		const vector<Script*>& vectorLeftScript = _left.target->GetScripts();
		const vector<Script*>& vectorRightScript = _right.target->GetScripts();

		int leftBaseSpeed = 0;
		int rightBaseSpeed = 0;

		for (size_t i = 0; i < vectorLeftScript.size(); ++i)
		{
			if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorRightScript[i]->GetScriptType())
			{
				leftBaseSpeed = ((MonsterScript*)vectorLeftScript[i])->GetSpeed();
			}
			else if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorLeftScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorLeftScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorLeftScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorLeftScript[i]->GetScriptType())
			{
				leftBaseSpeed = ((PlayerScript*)vectorLeftScript[i])->GetSpeed();
			}
		}

		for (size_t i = 0; i < vectorRightScript.size(); ++i)
		{
			if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorRightScript[i]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorRightScript[i]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorRightScript[i]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorRightScript[i]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorRightScript[i]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorRightScript[i]->GetScriptType())
			{
				rightBaseSpeed = ((MonsterScript*)vectorRightScript[i])->GetSpeed();
			}
			else if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorRightScript[i]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorRightScript[i]->GetScriptType())
			{
				rightBaseSpeed = ((PlayerScript*)vectorRightScript[i])->GetSpeed();
			}
		}

		return leftBaseSpeed > rightBaseSpeed;
	}
	else
	{
		return _left.speed > _right.speed;
	}
}

void PartyManager::CalOrder()
{
	srand((UINT)time(NULL));
	m_vectorOrder.clear();

	for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
	{
		if (nullptr == m_vectorPlayerParty[i])
			continue;

		const vector<Script*>& vectorScript = m_vectorPlayerParty[i]->GetScripts();

		for (size_t j = 0; j < vectorScript.size(); ++j)
		{
			if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorScript[j]->GetScriptType())
			{
				int baseSpeed = ((PlayerScript*)vectorScript[j])->GetSpeed();
				int additional = rand() % 8 + 1;
				int totalSpeed = baseSpeed + additional;

				m_vectorOrder.push_back(tOrder{ totalSpeed, m_vectorPlayerParty[i] });
			}
		}
	}

	for (size_t i = 0; i < m_vectorMonsterParty.size(); ++i)
	{
		if (nullptr == m_vectorMonsterParty[i])
			continue;

		const vector<Script*>& vectorScript = m_vectorMonsterParty[i]->GetScripts();

		for (size_t j = 0; j < vectorScript.size(); ++j)
		{
			if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorScript[j]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorScript[j]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorScript[j]->GetScriptType() || 
				(UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorScript[j]->GetScriptType())
			{
				int baseSpeed = ((MonsterScript*)vectorScript[j])->GetSpeed();
				int additional = rand() % 8 + 1;
				int totalSpeed = baseSpeed + additional;

				m_vectorOrder.push_back(tOrder{ totalSpeed, m_vectorMonsterParty[i] });
			}
		}
	}

	sort(m_vectorOrder.begin(), m_vectorOrder.end(), SortOrder);
}

void PartyManager::CheckDoor()
{
	const vector<GameObject*>& vectorDoor = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::INTERACTION);

	for (size_t i = 0; i < vectorDoor.size(); ++i)
	{
		if (vectorDoor[i]->GetScripts().empty())
			continue;

		if (vectorDoor[i]->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::DOORSCRIPT)
		{
			if (vectorDoor[i]->GetCollider2D() == nullptr)
				continue;

			if (vectorDoor[i]->GetCollider2D()->IsCollision() && KEY_TAP(KEY::W))
			{
				m_bEnter = true;
				m_targetDoor = vectorDoor[i];
			}
		}
	}
}

void PartyManager::CheckScene()
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();
	Scene* loadingScene = SceneManager::GetInst()->GetLoadingScene();

	if (curScene == loadingScene)
	{
		m_bChangeScene = true;
		ResetPlayerParty();
	}

	if (m_bChangeScene)
	{
		m_bChangeScene = false;

		const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);

		for (size_t i = 0; i < vectorPlayer.size(); ++i)
		{
			AddPlayerParty((UINT)i, vectorPlayer[i]);
		}
	}
}

void PartyManager::BattlePhaseManagement()
{
	BattlePhaseSound();

	for (size_t i = 0; i < m_vectorOrder.size(); ++i)
	{
		if (m_vectorOrder[i].target->IsDead())
		{
			++m_startCheck;
			continue;
		}

		const vector<Script*>& vectorScript = m_vectorOrder[i].target->GetScripts();

		if (m_vectorOrder[i].target->GetObjectType() == OBJECT_TYPE::PLAYER)
		{
			if (((PlayerScript*)vectorScript[0])->IsStun())
			{
				++m_startCheck;
				continue;
			}

			if (0 == i && m_bFirstTurn)
			{
				m_bFirstTurn = false;
				((PlayerScript*)vectorScript[0])->SetTurnStart(true);
				PlayerScript::m_selectPlayer = m_vectorOrder[i].target;
				((PlayerScript*)vectorScript[0])->SetSelect(true);
			}

			if (((PlayerScript*)vectorScript[0])->IsTurnEnd()) // 해당 턴에서 모든 캐릭터의 행동이 종료 되었을 때
			{
				++m_startCheck;
				((PlayerScript*)vectorScript[0])->SetTurnStart(false);
				((PlayerScript*)vectorScript[0])->SetTurnEnd(false);
				((PlayerScript*)vectorScript[0])->SetSelect(false);

				if (m_vectorOrder.size() - 1 == i)
					break;

				for (size_t j = i + 1; j < m_vectorOrder.size(); ++j)
				{
					const vector<Script*>& vectorNextScript = m_vectorOrder[j].target->GetScripts();

					if (m_vectorOrder[j].target->GetObjectType() == OBJECT_TYPE::MONSTER && !m_vectorOrder[i].target->IsDead())
					{
						if (m_vectorOrder[j].target->IsDead())
							continue;

						((MonsterScript*)vectorNextScript[0])->SetTurnStart(true);
						((MonsterScript*)vectorNextScript[0])->SelectTarget(m_vectorPlayerParty);
						break;
					}
					else if (m_vectorOrder[j].target->GetObjectType() == OBJECT_TYPE::PLAYER && !m_vectorOrder[i].target->IsDead())
					{
						if (m_vectorOrder[j].target->IsDead())
							continue;

						((PlayerScript*)vectorNextScript[0])->SetTurnStart(true);
						PlayerScript::m_selectPlayer = m_vectorOrder[j].target;
						((PlayerScript*)vectorNextScript[0])->SetSelect(true);
						break;
					}
				}
			}
		}
		
		if (m_vectorOrder[i].target->GetObjectType() == OBJECT_TYPE::MONSTER)
		{
			if (((MonsterScript*)vectorScript[0])->IsStun())
			{
				++m_startCheck;
				continue;
			}

			if (0 == i && m_bFirstTurn)
			{
				m_bFirstTurn = false;
				((MonsterScript*)vectorScript[0])->SetTurnStart(true);
				((MonsterScript*)vectorScript[0])->SelectTarget(m_vectorPlayerParty);
			}

			if (((MonsterScript*)vectorScript[0])->IsTurnEnd()) // 해당 턴에서 모든 캐릭터의 행동이 종료 되었을 때
			{
				++m_startCheck;
				((MonsterScript*)vectorScript[0])->SetTurnStart(false);
				((MonsterScript*)vectorScript[0])->SetTurnEnd(false);

				if (m_vectorOrder.size() - 1 == i)
					break;

				for (size_t j = i + 1; j < m_vectorOrder.size(); ++j)
				{
					const vector<Script*>& vectorNextScript = m_vectorOrder[j].target->GetScripts();

					if (m_vectorOrder[j].target->GetObjectType() == OBJECT_TYPE::MONSTER && !m_vectorOrder[i].target->IsDead())
					{
						if (m_vectorOrder[j].target->IsDead())
							continue;

						((MonsterScript*)vectorNextScript[0])->SetTurnStart(true);
						((MonsterScript*)vectorNextScript[0])->SelectTarget(m_vectorPlayerParty);
						break;
					}
					else if (m_vectorOrder[j].target->GetObjectType() == OBJECT_TYPE::PLAYER && !m_vectorOrder[i].target->IsDead())
					{
						if (m_vectorOrder[j].target->IsDead())
							continue;

						((PlayerScript*)vectorNextScript[0])->SetTurnStart(true);
						PlayerScript::m_selectPlayer = m_vectorOrder[j].target;
						((PlayerScript*)vectorNextScript[0])->SetSelect(true);
						break;
					}
				}
			}
		}
	}

	if (m_vectorOrder.size() <= (size_t)m_startCheck)
	{
		m_startCheck = 0;
		m_bTurn = true;
		m_bFirstTurn = true;
		++m_turnCount;
	}
}

void PartyManager::BattlePhaseSound()
{
	Ptr<Sound> explore_Intro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Explore_Intro");
	Ptr<Sound> explore_loop = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Explore_Loop");

	if (nullptr != explore_Intro)
	{
		explore_Intro->Stop();
	}

	if (nullptr != explore_loop)
	{
		explore_loop->Stop();
	}

	Ptr<Sound> battle_Intro = ResourceManager::GetInst()->Load<Sound>(L"Sound_Battle_Intro", L"sound\\dungeon\\Combat_Level1_Intro.wav");
	Ptr<Sound> battle_loop1 = ResourceManager::GetInst()->Load<Sound>(L"Sound_Battle_Loop1", L"sound\\dungeon\\Combat_Level1_Loop1.wav");
	Ptr<Sound> battle_loop2 = ResourceManager::GetInst()->Load<Sound>(L"Sound_Battle_Loop2", L"sound\\dungeon\\Combat_Level1_Loop2.wav");

	if (!battle_loop2->IsPlaying(m_battleSoundChannel))
	{
		m_battleSoundChannel = battle_Intro->Play(1);
		battle_Intro->SetVolume(0.1f, m_battleSoundChannel);

		if (!battle_Intro->IsPlaying(m_battleSoundChannel))
		{
			m_battleSoundChannel = battle_loop1->Play(1);
			battle_loop1->SetVolume(0.1f, m_battleSoundChannel);

			if (!battle_loop1->IsPlaying(m_battleSoundChannel))
			{
				m_battleSoundChannel = battle_loop2->Play(1);
				battle_loop2->SetVolume(0.1f, m_battleSoundChannel);
			}
		}
	}
}

void PartyManager::ExploreSound()
{
	if (SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode())
	{
		if (!m_bTown)
		{
			Ptr<Sound> battle_Intro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Battle_Intro");
			Ptr<Sound> battle_loop1 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Battle_Loop1");
			Ptr<Sound> battle_loop2 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Battle_Loop2");
			Ptr<Sound> town_Intro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Intro");
			Ptr<Sound> town_loop1 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Loop1");
			Ptr<Sound> town_loop2 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Loop2");
			Ptr<Sound> town_outro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Outro");

			if (nullptr != battle_Intro)
			{
				battle_Intro->Stop();
			}

			if (nullptr != battle_loop1)
			{
				battle_loop1->Stop();
			}

			if (nullptr != battle_loop2)
			{
				battle_loop2->Stop();
			}

			if (nullptr != town_Intro)
			{
				town_Intro->Stop();
			}

			if (nullptr != town_loop1)
			{
				town_loop1->Stop();
			}

			if (nullptr != town_loop2)
			{
				town_loop2->Stop();
			}

			if (nullptr != town_outro)
			{
				town_outro->Stop();
			}

			Ptr<Sound> explore_Intro = ResourceManager::GetInst()->Load<Sound>(L"Sound_Explore_Intro", L"sound\\dungeon\\Explore_Vaults_Level_1_Intro.wav");
			Ptr<Sound> explore_loop = ResourceManager::GetInst()->Load<Sound>(L"Sound_Explore_Loop", L"sound\\dungeon\\Explore_Vaults_Level_1_Loop.wav");

			if (!explore_loop->IsPlaying(m_exploreSoundChannel))
			{
				m_exploreSoundChannel = explore_Intro->Play(1);
				explore_Intro->SetVolume(0.2f, m_exploreSoundChannel);

				if (!explore_Intro->IsPlaying(m_exploreSoundChannel))
				{
					m_exploreSoundChannel = explore_loop->Play(1);
					explore_loop->SetVolume(0.2f, m_exploreSoundChannel);
				}
			}
		}
		else
		{
			Ptr<Sound> explore_Intro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Explore_Intro");
			Ptr<Sound> explore_loop = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Explore_Loop");

			if (nullptr != explore_Intro)
			{
				explore_Intro->Stop();
			}

			if (nullptr != explore_loop)
			{
				explore_loop->Stop();
			}

			Ptr<Sound> town_Intro = ResourceManager::GetInst()->Load<Sound>(L"Sound_Town_Intro", L"sound\\town\\Town_Stereo_Mix_INTRO.wav");
			Ptr<Sound> town_loop1 = ResourceManager::GetInst()->Load<Sound>(L"Sound_Town_Loop1", L"sound\\town\\Town_Stereo_Mix_LOOP_1.wav");
			Ptr<Sound> town_loop2 = ResourceManager::GetInst()->Load<Sound>(L"Sound_Town_Loop2", L"sound\\town\\Town_Stereo_Mix_LOOP_2.wav");
			Ptr<Sound> town_outro = ResourceManager::GetInst()->Load<Sound>(L"Sound_Town_Outro", L"sound\\town\\Town_Stereo_Mix_OUTRO.wav");

			if (!town_outro->IsPlaying(m_townSoundChannel))
			{
				m_townSoundChannel = town_Intro->Play(1);
				town_Intro->SetVolume(0.2f, m_townSoundChannel);

				if (!town_Intro->IsPlaying(m_townSoundChannel))
				{
					m_townSoundChannel = town_loop1->Play(1);
					town_loop1->SetVolume(0.2f, m_townSoundChannel);

					if (!town_loop1->IsPlaying(m_townSoundChannel))
					{
						m_townSoundChannel = town_loop2->Play(1);
						town_loop2->SetVolume(0.2f, m_townSoundChannel);

						if (!town_loop2->IsPlaying(m_townSoundChannel))
						{
							m_townSoundChannel = town_outro->Play(1);
							town_outro->SetVolume(0.2f, m_townSoundChannel);
						}
					}
				}
			}
		}
	}
}

void PartyManager::EnterTheDoor()
{
	int count = 0;
	int channel = 0;
	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");

	Ptr<Sound> exit = ResourceManager::GetInst()->Load<Sound>(L"Sound_Door_Close", L"sound\\dungeon\\gen_map_door_close.wav");

	if (((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::ENTERANCE ||
		((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::EXIT)
	{
		for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
		{
			if (nullptr == m_vectorPlayerParty[i])
				continue;

			Vector3 pos = m_vectorPlayerParty[i]->GetTransform()->GetLocalPos();
			pos.x += (m_targetDoor->GetTransform()->GetLocalPos().x - pos.x) * 0.6f * DELTATIME;
			pos.y += (m_targetDoor->GetTransform()->GetLocalPos().y - pos.y) * 0.6f * DELTATIME;

			if (((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::ENTERANCE)
			{
				if (3 != i)
				{
					if (nullptr != m_vectorPlayerParty[i + 1])
						pos.z = m_vectorPlayerParty[i + 1]->GetTransform()->GetLocalPos().z + 10.f;
				}
			}
			else if (((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::EXIT)
			{
				if (3 != i)
				{
					if (nullptr != m_vectorPlayerParty[i + 1])
						pos.z = m_vectorPlayerParty[i + 1]->GetTransform()->GetLocalPos().z - 10.f;
				}
			}

			if (m_vectorPlayerParty[i]->GetAnimator2D()->GetCurAnim()->GetName() != L"Walk")
			{
				m_vectorPlayerParty[i]->GetAnimator2D()->Play(L"Walk");
			}

			m_vectorPlayerParty[i]->GetTransform()->SetLocalPos(pos);

			if (nullptr != m_vectorPlayerParty[i])
				++count;
		}

		GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
		((CameraScript*)camera->GetScripts()[0])->SetEnter(true);
		((CameraScript*)camera->GetScripts()[0])->SetTargetDoor(m_targetDoor);

		float scale = camera->GetCamera()->GetScale();
		scale -= 0.2f * DELTATIME;

		if (scale > 0.5f)
			camera->GetCamera()->SetScale(scale);

		int index = count - 1;

		if (0 == count)
		{
			index = 0;
		}

		float range = light->GetLight2D()->GetLightInfo().range;
		range -= 200.f * DELTATIME;

		if (range > 0.f)
			light->GetLight2D()->SetRange(range);

		if (range < 0.f)
		{
			m_bNext = true;
			m_bEnter = false;
			m_bDoorSound = true;

			for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
			{
				if (nullptr != m_vectorPlayerParty[i])
					m_vectorPlayerParty[i]->GetAnimator2D()->Play(L"Idle");
			}

			((CameraScript*)camera->GetScripts()[0])->SetEnter(false);
			((CameraScript*)camera->GetScripts()[0])->SetTargetDoor(nullptr);
			camera->GetCamera()->SetScale(1.f);
			light->GetLight2D()->SetRange(1000.f);

			channel = exit->Play(1);
			exit->SetVolume(0.5f, channel);
		}
	}
	else if (((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::ROOM_ENTERANCE ||
		((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::ROOM_EXIT)
	{
		float flag = 1.f;

		if (((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::ROOM_ENTERANCE)
			flag = 1.f;
		else if (((DoorScript*)m_targetDoor->GetScripts()[0])->GetDoorType() == DOOR_TYPE::ROOM_EXIT)
			flag = -1.f;

		for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
		{
			if (nullptr == m_vectorPlayerParty[i])
				continue;

			Vector3 pos = m_vectorPlayerParty[i]->GetTransform()->GetLocalPos();
			pos.x += flag * 200.f * DELTATIME;

			if (nullptr == m_vectorPlayerParty[i]->GetAnimator2D()->GetCurAnim() ||
				m_vectorPlayerParty[i]->GetAnimator2D()->GetCurAnim()->GetName() != L"Walk")
			{
				m_vectorPlayerParty[i]->GetAnimator2D()->Play(L"Walk");
			}

			m_vectorPlayerParty[i]->GetTransform()->SetLocalPos(pos);

			if (nullptr != m_vectorPlayerParty[i])
				++count;
		}

		GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
		((CameraScript*)camera->GetScripts()[0])->SetEnter(true);
		((CameraScript*)camera->GetScripts()[0])->SetTargetDoor(m_targetDoor);

		int index = count - 1;

		if (0 == count)
		{
			index = 0;
		}

		float range = light->GetLight2D()->GetLightInfo().range;
		range -= 300.f * DELTATIME;

		if (range > 0.f)
			light->GetLight2D()->SetRange(range);

		if (10.f > range)
		{
			m_bNext = true;
			m_bEnter = false;
			m_bDoorSound = true;

			for (size_t i = 0; i < m_vectorPlayerParty.size(); ++i)
			{
				if (nullptr != m_vectorPlayerParty[i])
					m_vectorPlayerParty[i]->GetAnimator2D()->Play(L"Idle");
			}

			((CameraScript*)camera->GetScripts()[0])->SetEnter(false);
			((CameraScript*)camera->GetScripts()[0])->SetTargetDoor(nullptr);
			camera->GetCamera()->SetScale(1.f);
			light->GetLight2D()->SetRange(1000.f);

			
			channel = exit->Play(1);
			exit->SetVolume(0.5f, channel);
		}
	}
}

void PartyManager::StopSoundAll()
{
	if (SCENE_MODE::STOP == SceneManager::GetInst()->GetSceneMode())
	{
		PlayerScript::m_selectPlayer = nullptr;
		Ptr<Sound> town_Intro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Intro");
		Ptr<Sound> town_loop1 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Loop1");
		Ptr<Sound> town_loop2 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Loop2");
		Ptr<Sound> town_outro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Town_Outro");
		Ptr<Sound> battle_Intro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Battle_Intro");
		Ptr<Sound> battle_loop1 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Battle_Loop1");
		Ptr<Sound> battle_loop2 = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Battle_Loop2");
		Ptr<Sound> explore_Intro = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Explore_Intro");
		Ptr<Sound> explore_loop = ResourceManager::GetInst()->FindRes<Sound>(L"Sound_Explore_Loop");

		if (nullptr != battle_Intro)
		{
			battle_Intro->Stop();
		}

		if (nullptr != battle_loop1)
		{
			battle_loop1->Stop();
		}

		if (nullptr != battle_loop2)
		{
			battle_loop2->Stop();
		}

		if (nullptr != explore_Intro)
		{
			explore_Intro->Stop();
		}

		if (nullptr != explore_loop)
		{
			explore_loop->Stop();
		}

		if (nullptr != town_Intro)
		{
			town_Intro->Stop();
		}

		if (nullptr != town_loop1)
		{
			town_loop1->Stop();
		}

		if (nullptr != town_loop2)
		{
			town_loop2->Stop();
		}

		if (nullptr != town_outro)
		{
			town_outro->Stop();
		}
	}
}

GameObject* PartyManager::GetDestination()
{
	const vector<Script*>& doorScript = m_targetDoor->GetScripts();
	return ((DoorScript*)doorScript[0])->GetDestination();
}