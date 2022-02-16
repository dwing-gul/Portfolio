#include "stdafx.h"
#include "func.h"
#include "Obj.h"
#include "resource.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "Scene_Tool.h"
#include "Animator.h"
#include "Core.h"

void CreateObject(Obj* _obj, GROUP_TYPE _eType)
{
	tEvent createEvent = {};

	createEvent.eType = EVENT_TYPE::CREATE_OBJECT;
	createEvent.lParam = (DWORD_PTR)_obj;
	createEvent.wParam = (DWORD_PTR)_eType;

	EventManager::GetInst()->AddEvent(createEvent);
}

void DeleteObject(Obj* _obj)
{
	tEvent deleteEvent = {};

	deleteEvent.eType = EVENT_TYPE::DELETE_OBJECT;
	deleteEvent.lParam = (DWORD_PTR)_obj;

	EventManager::GetInst()->AddEvent(deleteEvent);
}

Vector2 RotateVector2(Vector2 _origin, float _degree)
{
	float theta = (_degree / 180.f) * float(M_PI);

	Vector2 output = Vector2(_origin.x * cosf(theta) - _origin.y * sinf(theta), _origin.x * sinf(theta) + _origin.y * cosf(theta));

	return output;
}

int CrossVectors(Vector2 _obj1, Vector2 _obj2)
{
	float cross = _obj1.x * _obj2.y - _obj2.x * _obj1.y;

	if (0.f < cross)
	{
		return -1;
	}
	else if (0.f == cross)
	{
		return 0;
	}
	else
		return 1;
}

void SaveWString(const wstring& _str, FILE* _file)
{
	BYTE len = (BYTE)_str.length();
	fwrite(&len, sizeof(BYTE), 1, _file);
	fwrite(_str.c_str(), sizeof(wchar_t), (size_t)len, _file);
}

void LoadWString(wstring& _str, FILE* _file)
{
	wchar_t buffer[255] = {};
	BYTE len = 0;
	fread(&len, sizeof(BYTE), 1, _file);
	fread(buffer, sizeof(wchar_t), (size_t)len, _file);
	_str = buffer;
}

void PlaySoundEffect(const wstring& _key, const wstring& _relativePath, bool _loop, float _pos, float _volume)
{
	Sound* sound = ResourceManager::GetInst()->FindSound(_key);
	if (nullptr == sound)
	{
		sound = ResourceManager::GetInst()->LoadSound(_key, _relativePath);
	}

	sound->SetPosition(_pos);
	sound->Play(_loop);
	sound->SetVolume(_volume);
}

void PlayBGM(const wstring& _key, const wstring& _relativePath, bool _loop, float _pos, float _volume)
{
	Sound* bgm = ResourceManager::GetInst()->FindSound(_key);

	if (nullptr == bgm)
	{
		bgm = ResourceManager::GetInst()->LoadSound(_key, _relativePath);
	}

	bgm->SetPosition(_pos);
	bgm->PlayToBGM(_loop);
	bgm->SetVolume(_volume);
}

void StopSound(const wstring& _key)
{
	Sound* sound = ResourceManager::GetInst()->FindSound(_key);
	if (nullptr == sound)
		return;

	sound->Stop(true);
}

LRESULT CALLBACK CreateTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdjustAnimProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CreateBackgroundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdjustBackgroundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdjustObjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdjustTileColliderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void MenuBridge(WPARAM _menuID)
{
	HWND hWnd = Core::GetInst()->GetWindowHandle();

	switch (_menuID)
	{
	case ID_CREATE_TILE :
		DialogBox(nullptr, MAKEINTRESOURCE(IDD_CREATE_TILE), hWnd, CreateTileProc);
		break;
	case ID_SAVE_TILE :
	{
		Scene_Tool* toolScene = (Scene_Tool*)SceneManager::GetInst()->GetCurScene();
		toolScene->SaveMapSelect();
	}
		break;
	case ID_LOAD_TILE :
	{
		Scene_Tool* toolScene = (Scene_Tool*)SceneManager::GetInst()->GetCurScene();
		toolScene->LoadMapSelect();
	}
		break;
	case ID_SAVE_ANIM :
	{
		Obj* anim = SceneManager::GetInst()->GetObjectByName(L"Sample", GROUP_TYPE::DEFAULT);
		if (nullptr != anim)
			anim->GetAnimator()->SaveAnim();
	}
		break;
	case ID_LOAD_ANIM :
	{
		Obj* anim = SceneManager::GetInst()->GetObjectByName(L"Sample", GROUP_TYPE::DEFAULT);

		if (nullptr != anim)
		{
			if (anim->GetAnimator() != nullptr)
			{
				anim->DeleteAnimator();
				anim->CreateAnimator();
			}
			anim->GetAnimator()->LoadAnim();
		}
	}
		break;
	case ID_CREATE_BACKGROUND :
		DialogBox(nullptr, MAKEINTRESOURCE(IDD_CREATE_BACKGROUND), hWnd, CreateBackgroundProc);
		break;
	case ID_ADJUST_SCALE_BG :
		DialogBox(nullptr, MAKEINTRESOURCE(IDD_ADJUST_SCALE_BG), hWnd, AdjustBackgroundProc);
		break;
	case ID_ANIM_ADJUST :
	{
		hDlg = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_ANIM_ADJUST), hWnd, AdjustAnimProc);
		ShowWindow(hDlg, SW_SHOW);
	}
		break;
	case ID_LOAD_TEXTURE :
	{
		Scene_Tool* toolScene = (Scene_Tool*)SceneManager::GetInst()->GetCurScene();
		toolScene->TextureLoad();
	}
		break;
	case ID_CREATE_OBJECT :
	{
		Scene_Tool* toolScene = (Scene_Tool*)SceneManager::GetInst()->GetCurScene();
		toolScene->CreateObjUI();
	}
		break;
	case ID_ADJUST_TILE_COLLIDER :
	{
		hDlg = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_ADJUST_TILE_COLLIDER), hWnd, AdjustTileColliderProc);
		ShowWindow(hDlg, SW_SHOW);
	}
		break;
	case ID_ADJUST_OBJECT :
	{
		hDlg = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_OBJ_ADJUST), hWnd, AdjustObjectProc);
		ShowWindow(hDlg, SW_SHOW);
	}
		break;
	case ID_SAVE_OBJ :
	{
		Scene_Tool* toolScene = (Scene_Tool*)SceneManager::GetInst()->GetCurScene();
		toolScene->SaveMonsterSelect();
	}
		break;
	case ID_LOAD_OBJ:
	{
		Scene_Tool* toolScene = (Scene_Tool*)SceneManager::GetInst()->GetCurScene();
		toolScene->LoadMonsterSelect();
	}
		break;
	}
}