#include "stdafx.h"
#include "Obj.h"
#include "Animator.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Texture.h"
#include "resource.h"

Obj::Obj() :
	m_pos(Vector2(0.f, 0.f)),
	m_scale(Vector2(100.f, 100.f)),
	m_type(GROUP_TYPE::END),
	m_texture(nullptr),
	m_animator(nullptr),
	m_collider(nullptr),
	m_bDead(false),
	m_bItem(false),
	m_bForAnimAdjust(false)
{
}

Obj::~Obj()
{
	if (nullptr != m_collider)
		delete m_collider;
	if (nullptr != m_animator)
		delete m_animator;
}

Obj::Obj(const Obj& _origin) :
	m_pos(_origin.m_pos),
	m_scale(_origin.m_scale),
	m_type(GROUP_TYPE::END),
	m_texture(_origin.m_texture),
	m_animator(nullptr),
	m_collider(nullptr),
	m_bItem(_origin.m_bItem),
	m_bDead(false),
	m_bForAnimAdjust(_origin.m_bForAnimAdjust)
{
	if (nullptr != _origin.m_animator)
	{
		m_animator = _origin.m_animator->Clone();
		m_animator->m_owner = this;
	}

	if (nullptr != _origin.m_collider)
	{
		m_collider = _origin.m_collider->Clone();
		m_collider->m_owner = this;
	}
}

void Obj::CreateCollider()
{
	if (nullptr != m_collider)
	{
		MessageBox(nullptr, L"충돌체 중복 생성", L"충돌체 생성 오류", MB_OK);
		return;
	}
	m_collider = new Collider;
	m_collider->m_owner = this;
}

void Obj::DeleteCollider()
{
	if (nullptr != m_collider)
	{
		delete m_collider;
		m_collider = nullptr;
	}
}

void Obj::CreateAnimator()
{
	assert(!m_animator);
	m_animator = new Animator;
	m_animator->m_owner = this;
}

void Obj::DeleteAnimator()
{
	if (nullptr != m_animator)
	{
		delete m_animator;
		m_animator = nullptr;
	}
}

void Obj::CreateTexture(const wstring& key, const wstring& relativePath)
{
	Texture* tex = ResourceManager::GetInst()->FindTexture(key);
	if (nullptr == tex)
		tex = ResourceManager::GetInst()->LoadTexture(key, relativePath);

	SetTexture(tex);
}

void Obj::lateUpdate()
{
	if (nullptr != m_collider)
		m_collider->lateUpdate();

	if (nullptr != m_animator)
		m_animator->lateUpdate();
}

void Obj::render(HDC _dc)
{
	if (nullptr != m_collider)
		m_collider->render(_dc);
}


void Obj::Load(FILE* _file)
{
	LoadWString(m_name, _file);
	SetName(m_name);
	Vector2 pos = Vector2(0, 0);
	fread(&pos, sizeof(Vector2), 2, _file);
	SetPos(pos);
	bool hasCollider = false;
	fread(&hasCollider, sizeof(bool), 1, _file);

	if (hasCollider)
	{
		CreateCollider();
		Vector2 offsetPos = Vector2(0, 0);
		Vector2 scale = Vector2(0, 0);
		fread(&offsetPos, sizeof(Vector2), 2, _file);
		fread(&scale, sizeof(Vector2), 2, _file);
		GetCollider()->SetOffsetPos(offsetPos);
		GetCollider()->SetScale(scale);
	}
}

LRESULT CALLBACK AdjustObjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			int x = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			int y = GetDlgItemInt(hWnd, IDC_EDIT2, nullptr, true);
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = temp;
			name += L"0";
			GetDlgItemText(hWnd, IDC_EDIT4, temp, 255);
			name += temp;

			Obj* target = SceneManager::GetInst()->GetObjectByName(name);

			if (nullptr == target)
			{
				MessageBox(nullptr, L"해당 오브젝트가 존재하지 않습니다.", L"오브젝트 탐색 에러", MB_OK);
			}
			else
			{
				target->SetPos(Vector2(x, y));
			}
		}
		else if (LOWORD(wParam) == ID_DELETE)
		{
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = temp;
			name += L"0";
			GetDlgItemText(hWnd, IDC_EDIT4, temp, 255);
			name += temp;

			SceneManager::GetInst()->DeleteObjectByName(name);
		}
		else if (LOWORD(wParam) == IDC_CHANGE_POS)
		{
			int x = GetDlgItemInt(hWnd, IDC_EDIT5, nullptr, true);
			int y = GetDlgItemInt(hWnd, IDC_EDIT6, nullptr, true);
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = temp;
			name += L"0";
			GetDlgItemText(hWnd, IDC_EDIT4, temp, 255);
			name += temp;

			Obj* target = SceneManager::GetInst()->GetObjectByName(name);

			if(nullptr == target)
				return (INT_PTR)TRUE;

			target->GetCollider();
			if(nullptr == target->GetCollider())
				return (INT_PTR)TRUE;

			UINT height = target->GetTexture()->GetHeight();
			float yPos = float(y - (int)height / 2);

			target->GetCollider()->SetOffsetPos(Vector2(float(x), yPos));
		}
		else if (LOWORD(wParam) == IDC_CHANGE_SCALE)
		{
			int x = GetDlgItemInt(hWnd, IDC_EDIT7, nullptr, true);
			int y = GetDlgItemInt(hWnd, IDC_EDIT8, nullptr, true);
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = temp;
			name += L"0";
			GetDlgItemText(hWnd, IDC_EDIT4, temp, 255);
			name += temp;

			Obj* target = SceneManager::GetInst()->GetObjectByName(name);

			if (nullptr == target)
				return (INT_PTR)TRUE;

			target->GetCollider();
			if (nullptr == target->GetCollider())
				return (INT_PTR)TRUE;

			target->GetCollider()->SetScale(Vector2(x, y));
		}
		else if (LOWORD(wParam) == IDC_DELETE_COLLIDER)
		{
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = temp;
			name += L"0";
			GetDlgItemText(hWnd, IDC_EDIT4, temp, 255);
			name += temp;

			Obj* target = SceneManager::GetInst()->GetObjectByName(name);
			if (nullptr == target)
				return (INT_PTR)TRUE;
			target->DeleteCollider();
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}