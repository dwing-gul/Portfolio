#include "pch.h"
#include "DoorScript.h"
#include <Engine/SceneManager.h>
#include <Engine/TimeManager.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/Animator2D.h>
#include <Engine/Animation2D.h>
#include <Engine/Light2D.h>
#include <Script/CameraScript.h>

DoorScript::DoorScript() :
	Script((UINT)SCRIPT_TYPE::DOORSCRIPT),
	m_dest(nullptr),
	m_type(DOOR_TYPE::ENTERANCE),
	m_bNext(false),
	m_bEnter(true),
	m_bSound(false)
{
}

DoorScript::~DoorScript()
{
}

void DoorScript::start()
{
	m_dest = SceneManager::GetInst()->FindObjByName(m_destName);
}

void DoorScript::update()
{
	if(nullptr != m_dest)
		m_destName = m_dest->GetName();
}

void DoorScript::lateUpdate()
{
}

void DoorScript::EnterTheDoor(vector<GameObject*>& _party)
{
	int count = 0;
	int channel = 0;
	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");

	Ptr<Sound> exit = ResourceManager::GetInst()->Load<Sound>(L"Sound_Door_Close", L"sound\\dungeon\\gen_map_door_close.wav");

	if (m_type == DOOR_TYPE::ENTERANCE ||
		m_type == DOOR_TYPE::EXIT)
	{
		for (size_t i = 0; i < _party.size(); ++i)
		{
			if (nullptr == _party[i])
				continue;

			Vector3 pos = _party[i]->GetTransform()->GetLocalPos();
			pos.x += (GetTransform()->GetLocalPos().x - pos.x) * 0.6f * DELTATIME;
			pos.y += (GetTransform()->GetLocalPos().y - pos.y) * 0.6f * DELTATIME;

			if (m_type == DOOR_TYPE::ENTERANCE)
			{
				if (3 != i)
				{
					if (nullptr != _party[i + 1])
						pos.z = _party[i + 1]->GetTransform()->GetLocalPos().z + 10.f;
				}
			}
			else if (m_type == DOOR_TYPE::EXIT)
			{
				if (3 != i)
				{
					if (nullptr != _party[i + 1])
						pos.z = _party[i + 1]->GetTransform()->GetLocalPos().z - 10.f;
				}
			}

			if (_party[i]->GetAnimator2D()->GetCurAnim()->GetName() != L"Walk")
			{
				_party[i]->GetAnimator2D()->Play(L"Walk");
			}

			_party[i]->GetTransform()->SetLocalPos(pos);

			if (nullptr != _party[i])
				++count;
		}

		GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
		((CameraScript*)camera->GetScripts()[0])->SetEnter(true);
		((CameraScript*)camera->GetScripts()[0])->SetTargetDoor(Object());

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
			m_bSound = true;

			for (size_t i = 0; i < _party.size(); ++i)
			{
				if (nullptr != _party[i])
					_party[i]->GetAnimator2D()->Play(L"Idle");
			}

			((CameraScript*)camera->GetScripts()[0])->SetEnter(false);
			((CameraScript*)camera->GetScripts()[0])->SetTargetDoor(nullptr);
			camera->GetCamera()->SetScale(1.f);
			light->GetLight2D()->SetRange(1000.f);

			channel = exit->Play(1);
			exit->SetVolume(0.5f, channel);
		}
	}
	else if (m_type == DOOR_TYPE::ROOM_ENTERANCE || m_type == DOOR_TYPE::ROOM_EXIT)
	{
		float flag = 1.f;

		if (m_type == DOOR_TYPE::ROOM_ENTERANCE)
			flag = 1.f;
		else if (m_type == DOOR_TYPE::ROOM_EXIT)
			flag = -1.f;

		for (size_t i = 0; i < _party.size(); ++i)
		{
			if (nullptr == _party[i])
				continue;

			Vector3 pos = _party[i]->GetTransform()->GetLocalPos();
			pos.x += flag * 150.f * DELTATIME;

			if (nullptr == _party[i]->GetAnimator2D()->GetCurAnim() ||
				_party[i]->GetAnimator2D()->GetCurAnim()->GetName() != L"Walk")
			{
				_party[i]->GetAnimator2D()->Play(L"Walk");
			}

			_party[i]->GetTransform()->SetLocalPos(pos);

			if (nullptr != _party[i])
				++count;
		}

		GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
		((CameraScript*)camera->GetScripts()[0])->SetEnter(true);
		((CameraScript*)camera->GetScripts()[0])->SetTargetDoor(Object());

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
			m_bSound = true;

			for (size_t i = 0; i < _party.size(); ++i)
			{
				if (nullptr != _party[i])
					_party[i]->GetAnimator2D()->Play(L"Idle");
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

void DoorScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_type, _file);
	SaveWStringToFile(m_destName, _file);
}

void DoorScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_type, _file);
	LoadWStringFromFile(m_destName, _file);
}
