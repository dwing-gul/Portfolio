#include "pch.h"
#include "CameraScript.h"
#include "DoorScript.h"
#include <Engine/TimeManager.h>
#include <Engine/SceneManager.h>
#include <Engine/KeyManager.h>
#include <Engine/GameObject.h>
#include <Engine/Transform.h>

CameraScript::CameraScript() :
	Script((UINT)SCRIPT_TYPE::CAMERASCRIPT),
	m_targetDoor(nullptr),
	m_bBattlePhase(false),
	m_bEnter(false),
	m_bMove(false),
	m_bFirst(true),
	m_leftEnd(0.f),
	m_rightEnd(0.f),
	m_floor(0),
	m_moveCount(1),
	m_bSkip(false)
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::start()
{
}

void CameraScript::update()
{
	if (m_bSkip)
		m_bSkip = false;

	if (KEY_TAP(KEY::N7))
	{
		m_floor = 2;
		Vector3 pos = GetTransform()->GetLocalPos();
		pos.x = 0.f;
		pos.y = 1600.f;
		GetTransform()->SetLocalPos(pos);
		m_bSkip = true;
	}
	else if (KEY_TAP(KEY::N8))
	{
		m_floor = 3;
		Vector3 pos = GetTransform()->GetLocalPos();
		pos.x = 2900.f;
		pos.y = 2400.f;
		GetTransform()->SetLocalPos(pos);
		m_bSkip = true;
	}
	else if (KEY_TAP(KEY::N9))
	{
		m_floor = 6;
		Vector3 pos = GetTransform()->GetLocalPos();
		pos.x = 0.f;
		pos.y = 4800.f;
		GetTransform()->SetLocalPos(pos);
		m_bSkip = true;
	}
	else if (KEY_TAP(KEY::N0))
	{
		m_floor = 12;
		Vector3 pos = GetTransform()->GetLocalPos();
		pos.x = 0.f;
		pos.y = 9600.f;
		GetTransform()->SetLocalPos(pos);
		m_bSkip = true;
	}

	if (m_bBattlePhase)
	{
		Vector3 pos = GetTransform()->GetLocalPos();
		vector<GameObject*> vectorPlayerClone;
		vector<GameObject*> vectorMonsterClone;
		const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
		for (size_t i = 0; i < vectorPlayer.size(); ++i)
		{
			vectorPlayerClone.push_back(vectorPlayer[i]);
		}
		const vector<GameObject*>& vectorMonster = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);
		for (size_t i = 0; i < vectorMonster.size(); ++i)
		{
			vectorMonsterClone.push_back(vectorMonster[i]);
		}

		Vector3 playerPos; 
		Vector3 monsterPos;

		if(!vectorPlayerClone.empty())
			playerPos = vectorPlayerClone[0]->GetTransform()->GetLocalPos();

		if(!vectorMonsterClone.empty())
			monsterPos = vectorMonsterClone[0]->GetTransform()->GetLocalPos();

		if (m_floor % 2 == 0)
		{
			playerPos.x = -150.f;
			vectorPlayerClone[0]->GetTransform()->SetLocalPos(playerPos);
		}

		pos.x = (playerPos.x + monsterPos.x) / 2.f;

		GetTransform()->SetLocalPos(pos);
	}
	else
	{
		if (m_bEnter)
		{
			const vector<Script*>& vectorScript = m_targetDoor->GetScripts();

			for (size_t i = 0; i < vectorScript.size(); ++i)
			{
				if (((DoorScript*)vectorScript[i])->GetDoorType() == DOOR_TYPE::EXIT ||
					((DoorScript*)vectorScript[i])->GetDoorType() == DOOR_TYPE::ENTERANCE)
				{
					Vector3 pos = GetTransform()->GetLocalPos();
					float targetXPos = m_targetDoor->GetTransform()->GetLocalPos().x;
					float xPos = pos.x;

					pos.x += (targetXPos - xPos) * DELTATIME;

					if (abs(pos.x - targetXPos) > 10.f)
						GetTransform()->SetLocalPos(pos);
				}
			}
			
			if (1 == m_moveCount)
			{
				m_bMove = true;
				--m_moveCount;
			}

			if (m_bMove)
			{
				m_bMove = false;

				for (size_t i = 0; i < vectorScript.size(); ++i)
				{
					if (vectorScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::DOORSCRIPT)
					{
						if (((DoorScript*)vectorScript[i])->GetDoorType() == DOOR_TYPE::EXIT ||
							((DoorScript*)vectorScript[i])->GetDoorType() == DOOR_TYPE::ROOM_EXIT)
						{
							--m_floor;

							if (m_floor < 0)
								m_floor = 0;
						}
						else if(((DoorScript*)vectorScript[i])->GetDoorType() == DOOR_TYPE::ENTERANCE ||
							((DoorScript*)vectorScript[i])->GetDoorType() == DOOR_TYPE::ROOM_ENTERANCE)
						{
							++m_floor;
						}
					}
				}
			}
		}
		else
		{
			m_moveCount = 1;

			const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);

			if (vectorPlayer.empty())
				return;

			Vector3 pos = GetTransform()->GetLocalPos();

			if (!m_bSkip)
			{
				pos.x = vectorPlayer[0]->GetTransform()->GetLocalPos().x + 150.f;
				pos.y = m_floor * 800.f;
			}
			else
			{
				Vector3 playerPos = pos;
				playerPos.x -= 150.f;
				vectorPlayer[0]->GetTransform()->SetLocalPos(playerPos);
			}

			if (0 == m_floor % 2)
			{
				m_leftEnd = 0.f;
				m_rightEnd = 0.f;
			}
			else if (1 == m_floor % 2)
			{
				m_leftEnd = 170.f;
				m_rightEnd = 3100.f;
			}

			if (pos.x < m_leftEnd)
				pos.x = m_leftEnd;

			if (pos.x > m_rightEnd)
				pos.x = m_rightEnd;

			GetTransform()->SetLocalPos(pos);
		}
	}
}

void CameraScript::lateUpdate()
{

}