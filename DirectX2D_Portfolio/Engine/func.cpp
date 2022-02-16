#include "pch.h"
#include "func.h"
#include "EventManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Collider2D.h"
#include "GameObject.h"

void SaveWStringToFile(const wstring& _str, FILE* _file)
{
	int len = (int)_str.length();
	SaveToFile(&len, _file);
	SaveToFile(_str.data(), _file, (UINT)len);
}

void LoadWStringFromFile(wstring& _str, FILE* _file)
{
	wchar_t buffer[255] = {};
	int len = 0;
	LoadFromFile(&len, _file);
	LoadFromFile(buffer, _file, (UINT)len);
	_str = buffer;
}

void ChangeSceneMode(SCENE_MODE _mode)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::CHANGE_SCENE_MODE;
	evn.lparam = (DWORD_PTR)_mode;
	EventManager::GetInst()->AddEvent(evn);
}

void ChangeScene(Scene* _nextScene)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::CHANGE_SCENE;
	evn.lparam = (DWORD_PTR)_nextScene;
	EventManager::GetInst()->AddEvent(evn);
}

string WStringToString(const wstring& _wstring)
{
	return string(_wstring.begin(), _wstring.end());
}

wstring StringToWString(const string& _string)
{
	return wstring(_string.begin(), _string.end());
}

bool IsMouseClicked(MOUSE_BUTTON _mouseButton, bool _hold)
{
	KEY type = KEY::KEY_END;
	if (MOUSE_BUTTON::LEFT == _mouseButton)
		type = KEY::LBUTTON;
	else if (MOUSE_BUTTON::RIGHT == _mouseButton)
		type = KEY::RBUTTON;

	if (_hold && KEY_HOLD(type))
	{
		return true;
	}

	if (!_hold && KEY_TAP(type))
	{
		return true;
	}

	if (KEY_AWAY(type))
	{
		return false;
	}

	return false;
}

static GameObject* target = nullptr;

void DragNDrop(GameObject* _target)
{
	if (nullptr != target && target != _target)
		return;

	if (IsMouseClicked(MOUSE_BUTTON::LEFT, true) && _target->GetCollider2D()->IsObjectHovered())
	{
		target = _target;
		Vector3 mousePos = KeyManager::GetInst()->GetMousePos();
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");

		if (nullptr != camera)
		{
			Vector3 cameraPos = camera->GetTransform()->GetLocalPos();
			mousePos += cameraPos;
		}

		_target->GetTransform()->SetLocalPos(mousePos);
	}

	if (!IsMouseClicked(MOUSE_BUTTON::LEFT, true))
	{
		target = nullptr;
	}
}
