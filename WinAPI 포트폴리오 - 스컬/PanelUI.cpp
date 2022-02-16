#include "stdafx.h"
#include "PanelUI.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "Core.h"
#include "ImageUI.h"
#include "HollowBox.h"
#include "Animator.h"
#include "Animation.h"
#include "Player.h"

PanelUI::PanelUI() :
	m_leftPadding(0.f),
	m_topPadding(0.f),
	m_rightPadding(0.f),
	m_bottomPadding(0.f),
	m_subjectType(SUBJECT_TYPE::NONE),
	m_itemType(ITEM_TYPE::NONE),
	m_skulType(SKUL::NONE),
	m_UICountX(0),
	m_UICountY(0),
	m_itemCount(0),
	m_bMove(false),
	m_bExit(false),
	m_bBitBlt(true)
{
}

PanelUI::~PanelUI()
{
}

void PanelUI::update()
{
	if (!IsDead())
	{
		if (IsLButtonDown() && m_bMove)
		{
			SetPos(MOUSE_POS - m_diff);
		}

		if (GetName() == L"EssenseBox")
		{
			Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

			if (nullptr != player->GetEssense())
			{
				SetLook(true);
			}
		}
	}

	UI::update();
}

void PanelUI::render(HDC _dc)
{
	Texture* tex = GetTexture();

	if (IsLook())
	{
		if (nullptr == tex)
		{
			HPEN pen = Core::GetInst()->GetPen(m_pen);
			HPEN prevPen = (HPEN)SelectObject(_dc, pen);
			HBRUSH brush = Core::GetInst()->GetBrush(m_brush);
			HBRUSH prevBrush = (HBRUSH)SelectObject(_dc, brush);

			Rectangle(_dc, int(GetFinalPos().x), int(GetFinalPos().y), int(GetFinalPos().x + GetScale().x), int(GetFinalPos().y + GetScale().y));

			SelectObject(_dc, prevBrush);
			SelectObject(_dc, prevPen);

			Obj* image = SceneManager::GetInst()->GetObjectByName(L"AnimTexture", GROUP_TYPE::UI);
			Obj* sample = SceneManager::GetInst()->GetObjectByName(L"Sample", GROUP_TYPE::DEFAULT);

			if (nullptr != image)
			{
				HollowBox* box = (HollowBox*)(((UI*)image)->GetChildByName(L"AbstractBox"));
				Vector2 finalPos = GetFinalPos();

				wchar_t xPos[255];
				wsprintf(xPos, TEXT("%d"), (int)box->GetPos().x);
				wchar_t yPos[255];
				wsprintf(yPos, TEXT("%d"), (int)box->GetPos().y);

				wchar_t xScale[255];
				wsprintf(xScale, TEXT("%d"), (int)box->GetScale().x);
				wchar_t yScale[255];
				wsprintf(yScale, TEXT("%d"), (int)box->GetScale().y);

				TextOut(_dc, int(finalPos.x) + 100, int(finalPos.y) + 10, L"X : ", (int)wcslen(L"X : "));
				TextOut(_dc, int(finalPos.x) + 120, int(finalPos.y) + 10, xPos, (int)wcslen(xPos));
				TextOut(_dc, int(finalPos.x) + 200, int(finalPos.y) + 10, L"Y : ", (int)wcslen(L"Y : "));
				TextOut(_dc, int(finalPos.x) + 220, int(finalPos.y) + 10, yPos, (int)wcslen(yPos));

				TextOut(_dc, int(finalPos.x) + 80, int(finalPos.y) + 30, L"Width : ", (int)wcslen(L"Width : "));
				TextOut(_dc, int(finalPos.x) + 120, int(finalPos.y) + 30, xScale, (int)wcslen(xScale));
				TextOut(_dc, int(finalPos.x) + 180, int(finalPos.y) + 30, L"Height : ", (int)wcslen(L"Height : "));
				TextOut(_dc, int(finalPos.x) + 220, int(finalPos.y) + 30, yScale, (int)wcslen(yScale));

				if (nullptr != sample)
				{
					Animator* anim = sample->GetAnimator();

					TextOut(_dc, int(finalPos.x) + 90, int(finalPos.y) + 50, L"애니메이션 명 : ", (int)wcslen(L"애니메이션 명 : "));
					map<wstring, Animation*> animation = anim->GetAllAnim();
					map<wstring, Animation*>::iterator iter = animation.begin();
					int j = 0;
					int k = 0;
					for (int i = 0; iter != animation.end(); ++iter)
					{
						int yPosToi = int(finalPos.y) + 50 + i;
						int yPosToj = int(finalPos.y) + 10 + j;
						int yPosTok = int(finalPos.y) + 10 + k;

						wchar_t frameLength[10] = {};
						int animSize = iter->second->GetFrameSize();
						_itow_s(animSize, frameLength, 10);

						if (finalPos.y + GetScale().y < yPosToi)
						{
							TextOut(_dc, int(finalPos.x) + 370, yPosToj, iter->first.c_str(), (int)iter->first.length());
							TextOut(_dc, int(finalPos.x) + 450, yPosToj, frameLength, (int)wcslen(frameLength));
							j += 20;
						}
						else
						{
							TextOut(_dc, int(finalPos.x) + 200, yPosToi, iter->first.c_str(), (int)iter->first.length());
							TextOut(_dc, int(finalPos.x) + 290, yPosToi, frameLength, (int)wcslen(frameLength));
							i += 20;
						}

						if (finalPos.y + GetScale().y < yPosToj)
						{
							TextOut(_dc, int(finalPos.x) + 500, yPosTok, iter->first.c_str(), (int)iter->first.length());
							TextOut(_dc, int(finalPos.x) + 590, yPosTok, frameLength, (int)wcslen(frameLength));
							k += 20;
						}
					}
				}
			}
		}
		else
		{
			Vector2 pos = GetFinalPos();
			UINT width = tex->GetWidth();
			UINT height = tex->GetHeight();

			if (m_bBitBlt)
				BitBlt(_dc, (int)pos.x, (int)pos.y, (int)GetScale().x, (int)GetScale().y, tex->GetTextureDC(), 0, 0, SRCCOPY);
			else
			{
				if (GetName() == L"Boss")
				{
					BLENDFUNCTION blend = {};
					blend.AlphaFormat = AC_SRC_ALPHA;
					blend.BlendFlags = 0;
					blend.BlendOp = AC_SRC_OVER;
					blend.SourceConstantAlpha = 255;

					AlphaBlend(_dc, (int)pos.x, (int)pos.y, int((float)width * 1.5f), int((float)height * 1.5f),
						tex->GetTextureDC(), 0, 0, width, height, blend);
				}
				else
				{
					BLENDFUNCTION blend = {};
					blend.AlphaFormat = AC_SRC_ALPHA;
					blend.BlendFlags = 0;
					blend.BlendOp = AC_SRC_OVER;
					blend.SourceConstantAlpha = 255;

					AlphaBlend(_dc, (int)pos.x, (int)pos.y, width, height,
						tex->GetTextureDC(), 0, 0, width, height, blend);
				}
			}
		}

		const vector<UI*>& vectorChildUI = GetChild();
		
		for (size_t i = 0; i < vectorChildUI.size(); ++i)
		{
			Vector2 finalPos = vectorChildUI[i]->GetFinalPos();
			Vector2 scale = vectorChildUI[i]->GetScale();

			vectorChildUI[i]->render(_dc);
		}
		
	}
}

void PanelUI::LButtonDown(Vector2 _mousePos)
{
	UI::LButtonDown(_mousePos);

	m_diff = GetLButtonDownPos() - GetPos();
}

void PanelUI::LButtonClick(Vector2 _mousePos)
{
	if (m_bExit)
	{
		UI* parent = GetParentUI();
		DeleteObject(parent);
	}
}

void PanelUI::ToolImageSetting(const wstring& _key, const wstring& _relativePath)
{
	Texture* tex = ResourceManager::GetInst()->FindTexture(_key.c_str());
	if (nullptr == tex)
		tex = ResourceManager::GetInst()->LoadTexture(_key.c_str(), _relativePath.c_str());
	assert(tex);

	m_UIScale = Vector2((TILE_SIZE / 2), (TILE_SIZE / 2));

	Vector2 vPanelScale = GetScale();

	int space = (int)(vPanelScale.x - (m_leftPadding + m_rightPadding));
	m_UICountX = space / (int)m_UIScale.x;
	m_insidePadding.x = (float)(space % (int)m_UIScale.x) / (float)(m_UICountX - 1);

	space = (int)(vPanelScale.y - (m_topPadding + m_bottomPadding));
	m_UICountY = space / (int)m_UIScale.y;
	m_insidePadding.y = (float)(space % (int)m_UIScale.y) / (float)(m_UICountY - 1);

	for (UINT col = 0; col < m_UICountY; ++col)
	{
		for (UINT row = 0; row < m_UICountX; ++row)
		{
			ImageUI* ImgUI = new ImageUI;
			ImgUI->SetTexture(tex);
			ImgUI->SetScale(Vector2(m_UIScale.x, m_UIScale.x));
			ImgUI->SetLT(Vector2(float(row * (TILE_SIZE / 2)), float(col * (TILE_SIZE / 2))));
			ImgUI->SetUIType(UI_TYPE::TILE);
			ImgUI->SetIsTile(true);
			ImgUI->SetTileIdx(int((col * m_UICountX)  + row));

			Vector2 pos = Vector2(m_leftPadding + (m_insidePadding.x + m_UIScale.x) * row
				, m_topPadding + (m_insidePadding.y + m_UIScale.y) * col);

			ImgUI->SetPos(pos);

			AddChild(ImgUI);
		}
	}
}

void PanelUI::ObjUISetting(Vector2 _pos, const wstring& _key, const wstring& _relativePath, GROUP_TYPE _type)
{
	ImageUI* obj = new ImageUI;
	obj->SetPos(Vector2(_pos.x, _pos.y));
	obj->CreateTexture(_key, _relativePath);
	obj->SetType(_type);
	obj->SetName(_key);
	obj->SetUIType(UI_TYPE::CREATE_OBJECT);
	float width = float(obj->GetTexture()->GetWidth());
	float height = float(obj->GetTexture()->GetHeight());
	obj->SetScale(Vector2(width, height));
	AddChild(obj);
}
