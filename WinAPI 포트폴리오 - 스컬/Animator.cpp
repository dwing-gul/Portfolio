#include "stdafx.h"
#include "Animator.h"
#include "Animation.h"
#include "Texture.h"
#include "PathManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Core.h"
#include "resource.h"
#include "HollowBox.h"

Animator::Animator() :
	m_owner(nullptr),
	m_curAnim(nullptr),
	m_bRepeat(false)
{
}

Animator::~Animator()
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.begin();

	for (; iter != m_mapAnim.end(); ++iter)
	{
		delete iter->second;
	}
}

Animator::Animator(const Animator& _origin) :
	m_owner(nullptr),
	m_curAnim(nullptr),
	m_bRepeat(_origin.m_bRepeat)
{
	map<wstring, Animation*> mapAnim = _origin.m_mapAnim;
	map<wstring, Animation*>::iterator iter = mapAnim.begin();

	for (; iter != mapAnim.end(); ++iter)
	{
		Animation* anim = iter->second->Clone();
		anim->m_animator = this;
		m_mapAnim.insert(make_pair(iter->first, anim));
	}

	m_curAnim = GetAnim(_origin.m_curAnim->GetName());
}

void Animator::CreateAnim(const wstring& _name, Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum, float _duration)
{
	Animation* anim = new Animation;

	anim->Create(_name, _texture, _leftTop, _slice, _frameNum, _duration);
	anim->m_animator = this;

	m_mapAnim.insert(make_pair(_name, anim));

	m_curAnim = anim;
}

void Animator::DeleteAnim(const wstring& _name, int _frameNum)
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.find(_name);
	if (m_mapAnim.end() == iter)
	{
		MessageBox(nullptr, L"존재하지 않는 애니메이션 입니다.", L"애니메이션 탐색 오류", MB_OK);
		return;
	}
	iter->second->Delete(_frameNum);

	if (iter->second->GetFrameSize() == 0)
	{
		delete iter->second;
		m_mapAnim.erase(iter);
	}
	
}

void Animator::DeleteAllAnim()
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.begin();

	for (; iter != m_mapAnim.end();)
	{
		int frameCount = iter->second->GetFrameSize();

		for (int i = 0; i < frameCount; ++i)
		{
			iter->second->Delete(i);
		}

		delete iter->second;
		iter = m_mapAnim.erase(iter);
	}
}

void Animator::ResettingAnim(const wstring& _name, Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum)
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.find(_name);
	if (m_mapAnim.end() == iter)
	{
		MessageBox(nullptr, L"존재하지 않는 애니메이션 입니다.", L"애니메이션 탐색 오류", MB_OK);
		return;
	}

	iter->second->Resetting(_texture, _leftTop, _slice, _frameNum);
}

void Animator::AdjustAnimPos(const wstring& _name, UINT _frameIdx, Vector2 _offset)
{
	Animation* target = GetAnim(_name);

	target->GetFrame(_frameIdx).offset = _offset;
}

void Animator::PlayAnim(const wstring& _name, bool _repeat)
{
	Animation* anim = GetAnim(_name);
	assert(anim);

	m_curAnim = anim;
	m_bRepeat = _repeat;
}

void Animator::MoveAnim(const wstring& _name)
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.find(_name);
	if (m_mapAnim.end() == iter)
	{
		return;
	}
	m_curAnim = iter->second;
}

void Animator::ChangeAnim(const wstring& _name)
{
	bool finish = m_curAnim->IsFinish();
	wstring name = _name;

	// 애니메이션이 끝났을 때 애니메이션 전환
	if (m_curAnim->IsFinish())
	{
		PlayAnim(_name, true);
	}
}

Animation* Animator::GetAnim(const wstring& _name)
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.find(_name);

	if (m_mapAnim.end() == iter)
	{
		return nullptr;
	}

	return iter->second;
}

wstring Animator::GetCurAnimName()
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.begin();

	for (; iter != m_mapAnim.end(); ++iter)
	{
		if (iter->second == m_curAnim)
			return iter->first;
	}

	return L"";
}

void Animator::lateUpdate()
{
	if (nullptr == m_curAnim)
		return;

	if (m_curAnim->IsFinish() && m_bRepeat)
	{
		m_curAnim->Reset();
	}

	m_curAnim->lateUpdate();
}

void Animator::render(HDC _dc)
{
	if (nullptr == m_curAnim)
		return;

	m_curAnim->render(_dc);
}

void Animator::EffectRender(HDC _dc)
{
	if (nullptr == m_curAnim)
		return;

	m_curAnim->EffectRender(_dc);
}

void Animator::SaveAnim()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
	strMapFilePath += L"Data\\Anim";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0애니메이션\0*.anim\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring contentPath = CONTENT_PATH;

		wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
		Save(relativePath);
	}
}

void Animator::LoadAnim()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
	strMapFilePath += L"Data\\Anim";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0애니메이션\0*.anim\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring contentPath = CONTENT_PATH;

		wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
		Load(relativePath);
	}
}

void Animator::Save(const wstring& _relativePath)
{
	wstring filePath = CONTENT_PATH;
	filePath += _relativePath;

	const wchar_t* test = wcsstr(_relativePath.c_str(), L".anim");
	
	if (test == NULL)
	{
		filePath += L".anim";
	}

	FILE* file = nullptr;

	errno_t err = _wfopen_s(&file, filePath.c_str(), L"wb");
	assert(file);

	int animCount = (int)m_mapAnim.size();
	fwrite(&animCount, sizeof(int), 1, file);

	map<wstring, Animation*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->Save(file);
	}

	fclose(file);
}

void Animator::Load(const wstring& _relativePath)
{
	wstring filePath = CONTENT_PATH;
	filePath += _relativePath;

	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, filePath.c_str(), L"rb");
	assert(file);

	int animCount = 0;
	fread(&animCount, sizeof(int), 1, file);

	for (int i = 0; i < animCount; ++i)
	{
		Animation* anim = new Animation;
		anim->Load(file);
		anim->m_animator = this;

		m_mapAnim.insert(make_pair(anim->GetName(), anim));
	}

	fclose(file);
}

LRESULT CALLBACK AdjustAnimProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Obj* sample = SceneManager::GetInst()->GetObjectByName(L"Sample", GROUP_TYPE::DEFAULT);
	Obj* image = SceneManager::GetInst()->GetObjectByName(L"AnimTexture", GROUP_TYPE::UI);
	Animator* anim = nullptr;
	if(nullptr != sample)
		anim = sample->GetAnimator();

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_FRAME_MOVE)
		{
			int frameIdx = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			wchar_t animname[255] = {};
			GetDlgItemText(hWnd, IDC_ANIM_NAME, animname, 255);

			HollowBox* box = (HollowBox*)(((UI*)image)->GetChildByName(L"AbstractBox"));

			if (nullptr == image)
				break;

			if (anim->GetAnim(animname) == nullptr)
			{
				anim->CreateAnim(animname, image->GetTexture(), box->GetFinalLT(), box->GetScale(), 0, 0.1f);
				anim->GetAnim(animname)->SetFrameIdx(0);
			}
			else
			{
				anim->MoveAnim(animname);

				if (anim->GetCurAnim()->GetFrameSize() <= frameIdx)
				{
					anim->GetCurAnim()->Create(animname, image->GetTexture(), box->GetFinalLT(), box->GetScale(), frameIdx, 0.1f);
				}
				
				anim->GetAnim(animname)->SetFrameIdx(frameIdx);
			}
			
		}
		else if (LOWORD(wParam) == IDC_FRAME_DELETE)
		{
			int frameIdx = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			wchar_t animname[255] = {};
			GetDlgItemText(hWnd, IDC_ANIM_NAME, animname, 255);

			if (nullptr == image)
				break;

			if (nullptr != anim)
			{
				anim->DeleteAnim(animname, frameIdx);
				if(anim->GetAnimCount() == 0)
					anim->NullCurAnim();
				else
					anim->GetAnim(animname)->SetFrameIdx(frameIdx - 1);
			}
		}
		else if (LOWORD(wParam) == IDC_MOVE_LT)
		{
			int frameIdx = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			wchar_t animname[255] = {};
			GetDlgItemText(hWnd, IDC_ANIM_NAME, animname, 255);
			
			HollowBox* box = (HollowBox*)(((UI*)image)->GetChildByName(L"AbstractBox"));

			if (nullptr == image)
				break;

			int xPos = GetDlgItemInt(hWnd, IDC_EDIT2, nullptr, true);
			int yPos = GetDlgItemInt(hWnd, IDC_EDIT3, nullptr, true);

			box->SetPos(Vector2(xPos, yPos));

			anim->ResettingAnim(animname, image->GetTexture(), box->GetFinalLT(), box->GetScale(), frameIdx);
		}
		else if (LOWORD(wParam) == IDC_CHANGE_SCALE)
		{
			int frameIdx = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			wchar_t animname[255] = {};
			GetDlgItemText(hWnd, IDC_ANIM_NAME, animname, 255);
			int xScale = GetDlgItemInt(hWnd, IDC_EDIT4, nullptr, true);
			int yScale = GetDlgItemInt(hWnd, IDC_EDIT5, nullptr, true);

			HollowBox* box = (HollowBox*)(((UI*)image)->GetChildByName(L"AbstractBox"));

			if (nullptr == image)
				break;

			box->SetScale(Vector2(xScale, yScale));

			anim->ResettingAnim(animname, image->GetTexture(), box->GetFinalLT(), box->GetScale(), frameIdx);
		}
		else if (LOWORD(wParam) == IDC_SET_OFFSET)
		{
			int offsetX = GetDlgItemInt(hWnd, IDC_EDIT6, nullptr, true);
			int offsetY = GetDlgItemInt(hWnd, IDC_EDIT7, nullptr, true);

			if (nullptr == image)
				break;

			int frame = anim->GetCurAnim()->GetCurFrameIdx();
			anim->GetCurAnim()->GetFrame(frame).offset.x = (float)offsetX;
			anim->GetCurAnim()->GetFrame(frame).offset.y = (float)offsetY;

		}
		else if (LOWORD(wParam) == IDC_SET_DURATION)
		{
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT8, temp, 255);

			float duration = (float)_wtof(temp);

			if (nullptr == image)
				break;

			int frame = anim->GetCurAnim()->GetCurFrameIdx();
			anim->GetCurAnim()->GetFrame(frame).duration = duration;
		}
		else if (LOWORD(wParam) == IDC_PLAY)
		{
			if (nullptr == image)
				break;

			anim->SetRepeat(true);
			anim->GetCurAnim()->Play();
		}
		else if (LOWORD(wParam) == IDC_PAUSE)
		{
			if (nullptr == image)
				break;

			TimeManager::GetInst()->Pause();
		}
		else if (LOWORD(wParam) == IDC_STOP)
		{
			
			if (nullptr == image)
				break;

			anim->GetCurAnim()->Stop();
			anim->SetRepeat(false);
		}
		else if (LOWORD(wParam) == IDC_DELETE_ANIM)
		{
			map<wstring, Animation*> animation = anim->GetAllAnim();
			map<wstring, Animation*>::iterator iter = animation.begin();
			if(animation.end() == iter)
				return (INT_PTR)TRUE;
			anim->DeleteAllAnim();
			anim->NullCurAnim();
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