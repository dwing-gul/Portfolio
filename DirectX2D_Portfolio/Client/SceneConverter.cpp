#include "pch.h"
#include "SceneConverter.h"
#include "ImGuiManager.h"
#include "PartyManager.h"
#include "TextUIManager.h"
#include "HierarchyGUI.h"
#include "ResourceManagerGUI.h"
#include "ToolScene.h"
#include "TownScene.h"
#include "DungeonScene.h"
#include "SceneConvertScript.h"
#include "PlayerWaitingScript.h"
#include <Engine/KeyManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/Collider2D.h>
#include <Engine/Animator2D.h>
#include <Engine/Prefab.h>
#include <Engine/Transform.h>
#include <Script/PlayerScript.h>
#include <Script/CameraScript.h>
#include <Script/DoorScript.h>
#include <Script/BattlePhaseScript.h>

SceneConverter::SceneConverter() :
	m_curSceneConfig(nullptr),
	m_nextSceneConfig(nullptr),
	m_bMove(false),
	m_bFinish(false),
	m_bFirst(true),
	m_bFirstAdd(true),
	m_bTool(false)
{
}

SceneConverter::~SceneConverter()
{
	Safe_Delete_Vector(m_vectorSceneConfig);
}

void SceneConverter::init()
{
	PartyManager::GetInst()->init();
	TextUIManager::GetInst()->init();
	m_vectorSceneConfig.push_back(new ToolScene);
	m_vectorSceneConfig.push_back(new TownScene);
	m_vectorSceneConfig.push_back(new DungeonScene);
	m_vectorSceneConfig[0]->CreateScene();
	m_curSceneConfig = m_vectorSceneConfig[0];
}

void SceneConverter::progress()
{
	if (m_bTool)
	{
		ListInit();
		m_bTool = false;
	}

	if (m_curSceneConfig->GetName() != "ToolScene")
	{
		Scene* curScene = SceneManager::GetInst()->GetCurScene();

		if (SceneManager::GetInst()->GetLoadingScene() != curScene)
		{
			curScene->SetSceneType(SCENE_TYPE::PLAY_STAGE);
			ChangeSceneMode(SCENE_MODE::PLAY);
		}

		if (m_curSceneConfig->GetName() == "TownScene")
		{
			PartyManager::GetInst()->SetTown(true);
		}
		else if (m_curSceneConfig->GetName() == "DungeonScene")
		{
			PartyManager::GetInst()->SetTown(false);
		}
	}

	if ((SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode() || m_curSceneConfig->GetName() == "TownScene") && m_bFirstAdd)
	{
		GameObject* progression = SceneManager::GetInst()->FindObjByName(L"Progression");

		if (nullptr != progression)
		{
			progression->AddComponent(new SceneConvertScript);
			m_bFirstAdd = false;
		}

		GameObject* playerSlot = SceneManager::GetInst()->FindObjByName(L"PlayerSlot1");
		if (nullptr != playerSlot)
		{
			playerSlot->AddComponent(new PlayerWaitingScript);
		}

		playerSlot = SceneManager::GetInst()->FindObjByName(L"PlayerSlot2");
		if (nullptr != playerSlot)
		{
			playerSlot->AddComponent(new PlayerWaitingScript);
		}

		playerSlot = SceneManager::GetInst()->FindObjByName(L"PlayerSlot3");
		if (nullptr != playerSlot)
		{
			playerSlot->AddComponent(new PlayerWaitingScript);
		}

		playerSlot = SceneManager::GetInst()->FindObjByName(L"PlayerSlot4");
		if (nullptr != playerSlot)
		{
			playerSlot->AddComponent(new PlayerWaitingScript);
		}
	}
	else if(!(SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode() || m_curSceneConfig->GetName() == "TownScene"))
	{
		m_bFirstAdd = true;
	}

	if (KEY_TAP(KEY::F1))
	{
		size_t index = 0;
		for (; index < m_vectorSceneConfig.size(); ++index)
		{
			if (m_curSceneConfig == m_vectorSceneConfig[index])
			{
				break;
			}
		}

		PartyManager::GetInst()->StopSoundAll();
		ChangeSceneMode(SCENE_MODE::STOP);

		if (index > 0)
			SceneConverter::GetInst()->MoveNextScene(m_vectorSceneConfig[index - 1]);
	}

	if (KEY_TAP(KEY::F2))
	{
		size_t index = 0;
		for (; index < m_vectorSceneConfig.size(); ++index)
		{
			if (m_curSceneConfig == m_vectorSceneConfig[index])
			{
				break;
			}
		}

		if (index < m_vectorSceneConfig.size())
			SceneConverter::GetInst()->MoveNextScene(m_vectorSceneConfig[index + 1]);
	}

	if (KEY_TAP(KEY::Q))
	{
		ForceQuitBattle();
	}

	if (m_bMove)
	{
		m_bMove = false;
	 
	 	if(nullptr != m_nextSceneConfig)
	 	{
			m_nextSceneConfig->CreateScene();
	 		m_curSceneConfig = m_nextSceneConfig;
	 		m_nextSceneConfig = nullptr;
	 	}

		if (m_curSceneConfig->GetName() == "ToolScene")
		{
			m_bTool = true;
		}
	}

	Scene* curScene = SceneManager::GetInst()->GetCurScene();
	Scene* loadingScene = SceneManager::GetInst()->GetLoadingScene();

	if (curScene == loadingScene)
	{
		ChangeSceneMode(SCENE_MODE::STOP);
		return;
	}

	if ((SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode() && m_curSceneConfig->GetName() == "ToolScene") || 
		m_curSceneConfig->GetName() == "DungeonScene")
	{
		if (m_bFirst)
		{
			m_bFirst = false;

			const vector<Prefab*>& vectorPlayer = PartyManager::GetInst()->GetWaitingPlayer();

			for (size_t i = 0; i < vectorPlayer.size(); ++i)
			{
				PartyManager::GetInst()->AddPlayerParty((UINT)i, vectorPlayer[i]->GetPrefabObject()->Clone());
			}
		}
	}
	else if (!(SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode() && m_curSceneConfig->GetName() == "DungeonScene"))
	{
		m_bFirst = true;
	}

	if (SCENE_MODE::PLAY == SceneManager::GetInst()->GetSceneMode())
	{
		CheckBattlePhase();

		PartyManager::GetInst()->update();
		TextUIManager::GetInst()->progress();

		if (PartyManager::GetInst()->IsNext())
		{
			ShiftPosition();
		}
	}
	else
	{
		PartyManager::GetInst()->StopSoundAll();
	}
}

SceneConfig* SceneConverter::FindSceneByName(const string& _name)
{
	for (size_t i = 0; i < m_vectorSceneConfig.size(); ++i)
	{
		if (m_vectorSceneConfig[i]->GetName() == _name)
		{
			return m_vectorSceneConfig[i];
		}
	}

	return nullptr;
}

void SceneConverter::CheckBattlePhase()
{
	vector<GameObject*> vectorPlayerClone;
	vector<GameObject*> vectorBattlePhaseClone;
	vector<GameObject*> vectorCameraClone;

	const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
	for (size_t i = 0; i < vectorPlayer.size(); ++i)
	{
		vectorPlayerClone.push_back(vectorPlayer[i]);
	}

	const vector<GameObject*>& vectorBattlePhase = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::INTERACTION);
	for (size_t i = 0; i < vectorBattlePhase.size(); ++i)
	{
		vectorBattlePhaseClone.push_back(vectorBattlePhase[i]);
	}

	const vector<GameObject*>& vectorCamera = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::CAMERA);
	for (size_t i = 0; i < vectorCamera.size(); ++i)
	{
		vectorCameraClone.push_back(vectorCamera[i]);
	}

	GameObject* interaction = nullptr;

	for (size_t i = 0; i < vectorBattlePhaseClone.size(); ++i)
	{
		const vector<Script*>& vectorBattlePhaseScript = vectorBattlePhaseClone[i]->GetScripts();

		for (size_t j = 0; j < vectorBattlePhaseScript.size(); ++j)
		{
			if (vectorBattlePhaseScript[j]->GetScriptType() == (UINT)SCRIPT_TYPE::BATTLEPHASESCRIPT &&
				!vectorPlayerClone.empty() &&
				abs(vectorBattlePhaseClone[i]->GetTransform()->GetLocalPos().y - vectorPlayerClone[0]->GetTransform()->GetLocalPos().y) < 200.f)
			{
				interaction = vectorBattlePhaseClone[i];
			}
		}
	}

	bool battle = PartyManager::GetInst()->IsBattle();

	if (nullptr != interaction && interaction->GetCollider2D()->IsCollision() && false == PartyManager::GetInst()->IsBattle())
	{
		PartyManager::GetInst()->SetBattle(true);

		for (size_t i = 0; i < vectorPlayerClone.size(); ++i)
		{
			const vector<Script*>& vectorScript = vectorPlayerClone[i]->GetScripts();

			for (size_t j = 0; j < vectorScript.size(); ++j)
			{
				if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorScript[j]->GetScriptType())
				{
					((PlayerScript*)vectorScript[j])->SetBattlePhase(true);
				}
			}
		}

		for (size_t i = 0; i < vectorCameraClone.size(); ++i)
		{
			const vector<Script*>& vectorScript = vectorCameraClone[i]->GetScripts();

			for (size_t j = 0; j < vectorScript.size(); ++j)
			{
				if ((UINT)SCRIPT_TYPE::CAMERASCRIPT == vectorScript[j]->GetScriptType())
				{
					((CameraScript*)vectorScript[j])->SetBattlePhase(true);
				}
			}
		}

		AppearMonster();

		if(!interaction->IsDead())
			Script::DeleteObject(interaction);
	}

	if (false == PartyManager::GetInst()->IsBattle() && m_bFinish)
	{
		m_bFinish = false;

		for (size_t i = 0; i < vectorPlayerClone.size(); ++i)
		{
			const vector<Script*>& vectorScript = vectorPlayerClone[i]->GetScripts();

			for (size_t j = 0; j < vectorScript.size(); ++j)
			{
				if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorScript[j]->GetScriptType())
				{
					((PlayerScript*)vectorScript[j])->SetBattlePhase(false);
				}
			}

			vectorPlayerClone[i]->GetAnimator2D()->Play(L"Idle");
		}

		for (size_t i = 0; i < vectorCameraClone.size(); ++i)
		{
			const vector<Script*>& vectorScript = vectorCameraClone[i]->GetScripts();

			for (size_t j = 0; j < vectorScript.size(); ++j)
			{
				if ((UINT)SCRIPT_TYPE::CAMERASCRIPT == vectorScript[j]->GetScriptType())
				{
					((CameraScript*)vectorScript[j])->SetBattlePhase(false);
				}
			}
		}
	}
}

void SceneConverter::AppearMonster()
{
	vector<Prefab*>& vectorWaiting = PartyManager::GetInst()->GetWaitingMonster();

	UINT max = 0;

	if (vectorWaiting.size() > 4)
		max = 4;
	else
		max = (UINT)vectorWaiting.size();

	for (UINT i = 0; i < max; ++i)
	{
		Prefab* monster = vectorWaiting.back();
		vectorWaiting.pop_back();

		PartyManager::GetInst()->AddMonsterParty(i, monster->GetPrefabObject()->Clone());
	}
}

void SceneConverter::ShiftPosition()
{
	GameObject* target = PartyManager::GetInst()->GetDestination();

	if (nullptr == target)
		return;

	const vector<GameObject*>& playerParty = PartyManager::GetInst()->GetPlayerParty();
	const vector<Script*>& doorScript = target->GetScripts();

	DOOR_TYPE type = ((DoorScript*)doorScript[0])->GetDoorType();

	if (DOOR_TYPE::ENTERANCE == type || DOOR_TYPE::ROOM_ENTERANCE == type)
	{
		Vector3 pos = playerParty[0]->GetTransform()->GetLocalPos();
		pos.x = target->GetTransform()->GetLocalPos().x;
		playerParty[0]->GetTransform()->SetLocalPos(pos); 
	}
	else if (DOOR_TYPE::EXIT == type)
	{
		int count = 0;

		for (size_t i = 0; i < playerParty.size(); ++i)
		{
			if (nullptr != playerParty[i])
				++count;
		}

		Vector3 pos = playerParty[count - 1]->GetTransform()->GetLocalPos();
		pos.x = target->GetTransform()->GetLocalPos().x;
		playerParty[count - 1]->GetTransform()->SetLocalPos(pos);

		int index = count - 2;

		if (count < 2)
		{
			index = count;
		}

		for(int i = index; i >= 0; --i)
		{
			if (nullptr == playerParty[i])
				continue;

			Vector3 pos = playerParty[i]->GetTransform()->GetLocalPos();

			if(nullptr != playerParty[i + 1])
				playerParty[i]->GetTransform()->SetLocalPos(Vector3(playerParty[i + 1]->GetTransform()->GetLocalPos().x + 100.f, pos.y, pos.z));

			if (0 == i)
				break;
		}
	}
	else if (DOOR_TYPE::ROOM_EXIT == type)
	{
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 cameraPos = camera->GetTransform()->GetLocalPos();
		Vector3 pos = playerParty[0]->GetTransform()->GetLocalPos();
		pos.x = -150.f;
		playerParty[0]->GetTransform()->SetLocalPos(pos);
	}

	PartyManager::GetInst()->SetNext(false);
}

void SceneConverter::ListInit()
{
	GUI* gui = ImGuiManager::GetInst()->FindGUI("Hierarchy");
	gui->init();

	gui = ImGuiManager::GetInst()->FindGUI("Resource");
	gui->init();
}

void SceneConverter::ForceQuitBattle()
{
	m_bFinish = false;

	vector<GameObject*> vectorPlayerClone;
	vector<GameObject*> vectorMonsterClone;
	vector<GameObject*> vectorCameraClone;

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

	const vector<GameObject*>& vectorCamera = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::CAMERA);
	for (size_t i = 0; i < vectorCamera.size(); ++i)
	{
		vectorCameraClone.push_back(vectorCamera[i]);
	}

	for (size_t i = 0; i < vectorPlayerClone.size(); ++i)
	{
		const vector<Script*>& vectorScript = vectorPlayerClone[i]->GetScripts();

		for (size_t j = 0; j < vectorScript.size(); ++j)
		{
			if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorScript[j]->GetScriptType() ||
				(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorScript[j]->GetScriptType())
			{
				((PlayerScript*)vectorScript[j])->SetBattlePhase(false);
			}
		}

		vectorPlayerClone[i]->GetAnimator2D()->Play(L"Idle");
	}

	for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
	{
		if (nullptr != vectorMonsterClone[i] && !(vectorMonsterClone[i]->IsDead()))
		{
			Script::DeleteObject(vectorMonsterClone[i]);
		}
	}

	for (size_t i = 0; i < vectorCameraClone.size(); ++i)
	{
		const vector<Script*>& vectorScript = vectorCameraClone[i]->GetScripts();

		for (size_t j = 0; j < vectorScript.size(); ++j)
		{
			if ((UINT)SCRIPT_TYPE::CAMERASCRIPT == vectorScript[j]->GetScriptType())
			{
				((CameraScript*)vectorScript[j])->SetBattlePhase(false);
			}
		}
	}
}
