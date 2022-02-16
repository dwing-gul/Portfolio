#include "stdafx.h"
#include "ItemImageUI.h"
#include "Animator.h"
#include "Core.h"
#include "PanelUI.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "SceneManager.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Texture.h"
#include "Player.h"
#include "Item.h"

ItemImageUI::ItemImageUI() :
	m_itemType(SUBJECT_TYPE::ITEM),
	m_itemStyle(ITEM_TYPE::NONE),
	m_itemGrade(GRADE::NORMAL),
	m_skulType(SKUL::NONE),
	m_bCreate(false),
	m_bDestroy(false),
	m_timer(1.f)
{
	
}

ItemImageUI::~ItemImageUI()
{
}

void ItemImageUI::update()
{
	if (!IsDead())
	{
		if (nullptr != GetAnimator())
		{
			GetAnimator()->PlayAnim(L"Idle", true);
		}
	
		if (SUBJECT_TYPE::ITEM == m_itemType || SUBJECT_TYPE::ESSENCE == m_itemType)
		{
			const vector<UI*>& vectorChildUI = GetParentUI()->GetChild();
			for (size_t i = 0; i < vectorChildUI.size(); ++i)
			{
				if (vectorChildUI[i]->GetName() == L"ItemInfo")
				{

					if (IsMouseOn())
					{
						if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_PANEL)
						{
							if (((PanelUI*)vectorChildUI[i])->GetItemType() == m_itemStyle)
							{
								if (!m_bCreate)
								{
									ItemUISetting(vectorChildUI[i]);
								}

								vectorChildUI[i]->SetLook(true);
								vectorChildUI[i]->update();
							}
						}
						else if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_TEXT)
						{
							if (((TextUI*)vectorChildUI[i])->GetItemType() == m_itemStyle)
							{
								if (!m_bCreate)
								{
									ItemUISetting(vectorChildUI[i]);
								}

								vectorChildUI[i]->SetLook(true);
								vectorChildUI[i]->update();
							}
						}
					}
					else
					{
						if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_PANEL)
						{
							if (((PanelUI*)vectorChildUI[i])->GetItemType() == m_itemStyle)
							{
								vectorChildUI[i]->SetLook(false);
							}
						}
						else if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_TEXT)
						{
							if (((TextUI*)vectorChildUI[i])->GetItemType() == m_itemStyle)
							{
								vectorChildUI[i]->SetLook(false);
							}
						}
					}
				}
			}
		}

		if (SUBJECT_TYPE::SKUL == m_itemType)
		{
			const vector<UI*>& vectorChildUI = GetParentUI()->GetChild();
			for (size_t i = 0; i < vectorChildUI.size(); ++i)
			{
				if (vectorChildUI[i]->GetName() == L"SkulInfo")
				{
					if (IsMouseOn())
					{
						if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_PANEL)
						{
							if (((PanelUI*)vectorChildUI[i])->GetSkulType() == m_skulType)
							{
								if (!m_bCreate)
								{
									ItemUISetting(vectorChildUI[i]);
								}

								vectorChildUI[i]->SetLook(true);
								vectorChildUI[i]->update();
							}
						}
						else if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_TEXT)
						{
							if (((TextUI*)vectorChildUI[i])->GetSkulType() == m_skulType)
							{
								if (!m_bCreate)
								{
									ItemUISetting(vectorChildUI[i]);
								}

								vectorChildUI[i]->SetLook(true);
								vectorChildUI[i]->update();
							}
						}
					}
					else
					{
						if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_PANEL)
						{
							if (((PanelUI*)vectorChildUI[i])->GetSkulType() == m_skulType)
							{
								vectorChildUI[i]->SetLook(false);
							}
						}
						else if (vectorChildUI[i]->GetUIType() == UI_TYPE::ITEM_INFO_TEXT)
						{
							if (((TextUI*)vectorChildUI[i])->GetSkulType() == m_skulType)
							{
								vectorChildUI[i]->SetLook(false);
							}
						}
					}
				}
			}

			int a = 0;
		}
	}
}

void ItemImageUI::lateUpdate()
{
	if (IsMouseOn())
	{
		if (KEY_HOLD(KEY::F))
		{
			m_timer -= 0.01f;

			if (m_timer < 0.f)
			{
				m_bDestroy = true;
			}
		}

		if (KEY_AWAY(KEY::F))
		{
			m_timer = 1.f;
		}
	}

	if (m_bDestroy)
	{
		if (SUBJECT_TYPE::ITEM == m_itemType)
		{
			Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

			vector<Item*>& vectorItem = player->GetItems();
			vector<UI*>& vectorChildInventory = GetParentUI()->GetChildModifiable();
			vector<Item*>::iterator iterItem = vectorItem.begin();
			vector<UI*>::iterator iterUI = vectorChildInventory.begin();

			for (; iterItem != vectorItem.end(); )
			{
				if ((*iterItem)->GetItemStyle() == m_itemStyle)
				{
					(*iterItem)->DestroyItem(m_itemStyle);
					delete* iterItem;
					iterItem = vectorItem.erase(iterItem);
				}
				else
				{
					++iterItem;
				}
			}

			((PanelUI*)GetParentUI())->SubItemCount();

			for (; iterUI != vectorChildInventory.end(); )
			{
				if ((*iterUI)->GetName() == L"ItemInfo")
				{
					if ((*iterUI)->GetUIType() == UI_TYPE::ITEM_INFO_PANEL)
					{
						if (((PanelUI*)(*iterUI))->GetItemType() == m_itemStyle)
						{
							delete* iterUI;
							iterUI = vectorChildInventory.erase(iterUI);
						}
						else
						{
							++iterUI;
						}
					}
					else if ((*iterUI)->GetUIType() == UI_TYPE::ITEM_INFO_TEXT)
					{
						if (((TextUI*)(*iterUI))->GetItemType() == m_itemStyle)
						{
							delete* iterUI;
							iterUI = vectorChildInventory.erase(iterUI);
						}
						else
						{
							++iterUI;
						}
					}
				}
				else
				{
					++iterUI;
				}
			}

			iterUI = vectorChildInventory.begin();
			for (; iterUI != vectorChildInventory.end(); )
			{
				if ((*iterUI)->GetName() == GetName())
				{
					delete* iterUI;
					iterUI = vectorChildInventory.erase(iterUI);
				}
				else
					++iterUI;
			}

			m_bDestroy = false;
		}
	}

	UI::lateUpdate();
}

void ItemImageUI::render(HDC _dc)
{
	if (nullptr != GetAnimator())
	{
		GetAnimator()->render(_dc);
	}

#ifdef _DEBUG
	if (nullptr == GetTexture())
	{
		HPEN pen = Core::GetInst()->GetPen(PEN_TYPE::GREEN);
		HBRUSH brush = Core::GetInst()->GetBrush(BRUSH_TYPE::HOLLOW);

		HPEN prevPen = (HPEN)SelectObject(_dc, pen);
		HBRUSH prevBrush = (HBRUSH)SelectObject(_dc, brush);

		Rectangle(_dc, int(GetFinalPos().x), int(GetFinalPos().y), int(GetFinalPos().x + GetScale().x), int(GetFinalPos().y + GetScale().y));

		SelectObject(_dc, prevBrush);
		SelectObject(_dc, prevPen);
	}
#endif
}

void ItemImageUI::ItemUISetting(UI* _parentUI)
{
	float width = 0;
	float height = 0;

	if (ITEM_TYPE::RING_OF_WIND == m_itemStyle)
	{
		PanelUI* portrait = new PanelUI;
		portrait->CreateTexture(L"RingOfWind", L"Texture\\UI\\RingOfWindIcon.bmp");
		width = (float)portrait->GetTexture()->GetWidth();
		height = (float)portrait->GetTexture()->GetHeight();
		portrait->SetScale(Vector2(width, height));
		portrait->SetPos(Vector2(780.f, 12.f));
		portrait->IsBitBlt(false);
		portrait->SetLook(false);
		portrait->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		portrait->SetSubjectType(SUBJECT_TYPE::ITEM);
		portrait->SetItemType(m_itemStyle);
		portrait->SetName(L"ItemInfo");
		_parentUI->GetParentUI()->AddChild(portrait);

		PanelUI* engrave = new PanelUI;
		engrave->CreateTexture(L"Scamper", L"Texture\\UI\\ScamperIcon.bmp");
		width = (float)engrave->GetTexture()->GetWidth();
		height = (float)engrave->GetTexture()->GetHeight();
		engrave->SetScale(Vector2(width, height));
		engrave->IsBitBlt(false);
		engrave->SetSubjectType(SUBJECT_TYPE::ITEM);
		engrave->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		engrave->SetItemType(m_itemStyle);
		engrave->SetPos(Vector2(145.f, 37.f));
		_parentUI->AddChild(engrave);

		engrave = engrave->Clone();
		engrave->CreateTexture(L"Soar", L"Texture\\UI\\SoarIcon.bmp");
		width = (float)engrave->GetTexture()->GetWidth();
		height = (float)engrave->GetTexture()->GetHeight();
		engrave->SetSubjectType(SUBJECT_TYPE::ITEM);
		engrave->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		engrave->SetItemType(m_itemStyle);
		engrave->SetScale(Vector2(width, height));
		engrave->SetPos(Vector2(462.f, 37.f));
		_parentUI->AddChild(engrave);

		TextUI* info = new TextUI;
		info->SetText(L"�ٶ��� ����");
		info->SetPos(Vector2(850.f, 160.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Ϲ�");
		info->SetPos(Vector2(600.f, 170.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"��â�� ������ �ƴ����� �����ڵ鿡�Դ� �׾߸��� �α� �����̴�.");
		info->SetPos(Vector2(850.f, 200.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"���ݼӵ��� 15%, �̵��ӵ��� 10% �����մϴ�.");
		info->SetPos(Vector2(850.f, 270.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"����");
		info->SetPos(Vector2(170.f, 95.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���");
		info->SetPos(Vector2(485.f, 95.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���ݼӵ��� �̵��ӵ� 10% ����");
		info->SetPos(Vector2(170.f, 160.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"������ ���� �� ���� ���ݷ� 35% ����.");
		info->SetPos(Vector2(490.f, 160.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���� �� 1�� �� �����");
		info->SetPos(Vector2(434.f, 180.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);
	}
	else if (ITEM_TYPE::ADMISSION_NOTICE_SEAT == m_itemStyle)
	{
		PanelUI* portrait = new PanelUI;
		portrait->CreateTexture(L"AdmissionNoticeSeat", L"Texture\\UI\\AdmissionNoticeSeatIcon.bmp");
		width = (float)portrait->GetTexture()->GetWidth();
		height = (float)portrait->GetTexture()->GetHeight();
		portrait->SetScale(Vector2(width, height));
		portrait->SetPos(Vector2(785.f, 14.f));
		portrait->IsBitBlt(false);
		portrait->SetLook(false);
		portrait->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		portrait->SetSubjectType(SUBJECT_TYPE::ITEM);
		portrait->SetItemType(m_itemStyle);
		portrait->SetName(L"ItemInfo");
		_parentUI->GetParentUI()->AddChild(portrait);

		PanelUI* engrave = new PanelUI;
		engrave->CreateTexture(L"Heart", L"Texture\\UI\\HeartIcon.bmp");
		width = (float)engrave->GetTexture()->GetWidth();
		height = (float)engrave->GetTexture()->GetHeight();
		engrave->SetScale(Vector2(width, height));
		engrave->IsBitBlt(true);
		engrave->SetSubjectType(SUBJECT_TYPE::ITEM);
		engrave->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		engrave->SetItemType(m_itemStyle);
		engrave->SetPos(Vector2(145.f, 37.f));
		_parentUI->AddChild(engrave);

		engrave = engrave->Clone();
		engrave->CreateTexture(L"Brave", L"Texture\\UI\\BraveIcon.bmp");
		width = (float)engrave->GetTexture()->GetWidth();
		height = (float)engrave->GetTexture()->GetHeight();
		engrave->SetSubjectType(SUBJECT_TYPE::ITEM);
		engrave->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		engrave->SetItemType(m_itemStyle);
		engrave->SetScale(Vector2(width, height));
		engrave->SetPos(Vector2(462.f, 37.f));
		_parentUI->AddChild(engrave);

		TextUI* info = new TextUI;
		info->SetText(L"�Կ�������");
		info->SetPos(Vector2(850.f, 160.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Ϲ�");
		info->SetPos(Vector2(600.f, 170.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Կ� �δ밡 �����Ǿ�����, ���� ���� ������ �Ժ��� �����մϴ�.");
		info->SetPos(Vector2(850.f, 200.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�������ݷ� �� �������ݷ��� 10% �����մϴ�.");
		info->SetPos(Vector2(850.f, 270.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"����");
		info->SetPos(Vector2(170.f, 95.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���");
		info->SetPos(Vector2(485.f, 95.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"�ִ�ü�� 50 ����");
		info->SetPos(Vector2(170.f, 160.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���� ������ 50% ����");
		info->SetPos(Vector2(490.f, 160.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);
	}
	else if (SUBJECT_TYPE::ESSENCE == m_itemType)
	{
		PanelUI* portrait = new PanelUI;
		portrait->CreateTexture(L"OgreIcon", L"Texture\\UI\\OgreIcon.bmp");
		width = (float)portrait->GetTexture()->GetWidth();
		height = (float)portrait->GetTexture()->GetHeight();
		portrait->SetScale(Vector2(width, height));
		portrait->SetPos(Vector2(790.f, 20.f));
		portrait->IsBitBlt(false);
		portrait->SetLook(false);
		portrait->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		portrait->SetSubjectType(SUBJECT_TYPE::ESSENCE);
		portrait->SetName(L"ItemInfo");
		_parentUI->GetParentUI()->AddChild(portrait);

		TextUI* info = new TextUI;
		info->SetText(L"�����");
		info->SetPos(Vector2(850.f, 160.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"����");
		info->SetPos(Vector2(600.f, 170.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"40��");
		info->SetPos(Vector2(1090.f, 170.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�ܴ��� �Ǻο� �Ŵ��� ��ġ�� ���� �׵�� �¼����� �ڴ� ���� ������.");
		info->SetPos(Vector2(850.f, 200.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�ִ� ü���� 40 �����մϴ�.");
		info->SetPos(Vector2(850.f, 270.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"���� �Ǻ�");
		info->SetPos(Vector2(270.f, 20.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"10�ʰ� �޴� �������� 40% �����մϴ�.");
		info->SetPos(Vector2(260.f, 70.f));
		info->SetName(L"ItemInfo");
		info->SetSubjectType(SUBJECT_TYPE::ITEM);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetItemType(m_itemStyle);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);
	}
	else if (SKUL::BASIC == m_skulType)
	{
		PanelUI* portrait = new PanelUI;
		portrait->CreateTexture(L"BasicSkulPortrait", L"Texture\\UI\\BasicSkulIcon.bmp");
		width = (float)portrait->GetTexture()->GetWidth();
		height = (float)portrait->GetTexture()->GetHeight();
		portrait->SetScale(Vector2(width, height));
		portrait->SetPos(Vector2(783.f, 15.f));
		portrait->IsBitBlt(false);
		portrait->SetLook(false);
		portrait->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		portrait->SetSubjectType(SUBJECT_TYPE::SKUL);
		portrait->SetName(L"SkulInfo");
		portrait->SetSkulType(m_skulType);
		_parentUI->GetParentUI()->AddChild(portrait);

		PanelUI* skil = new PanelUI;
		skil->CreateTexture(L"ThrowingSkulIcon", L"Texture\\UI\\ThrowingSkulIcon.bmp");
		width = (float)skil->GetTexture()->GetWidth();
		height = (float)skil->GetTexture()->GetHeight();
		skil->SetScale(Vector2(width, height));
		skil->IsBitBlt(true);
		skil->SetSubjectType(SUBJECT_TYPE::SKUL);
		skil->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		skil->SetPos(Vector2(145.f, 37.f));
		skil->SetSkulType(m_skulType);
		_parentUI->AddChild(skil);

		skil = skil->Clone();
		skil->CreateTexture(L"HeadIsBodyIcon", L"Texture\\UI\\HeadIsBodyIcon.bmp");
		width = (float)skil->GetTexture()->GetWidth();
		height = (float)skil->GetTexture()->GetHeight();
		skil->SetSubjectType(SUBJECT_TYPE::SKUL);
		skil->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		skil->SetScale(Vector2(width, height));
		skil->SetPos(Vector2(462.f, 37.f));
		skil->SetSkulType(m_skulType);
		_parentUI->AddChild(skil);

		TextUI* info = new TextUI;
		info->SetText(L"��Ʋ��");
		info->SetPos(Vector2(850.f, 160.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetSkulType(m_skulType);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Ϲ�");
		info->SetPos(Vector2(600.f, 170.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�뷱��");
		info->SetPos(Vector2(1085.f, 170.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"���ռ� ������ ���� ���̷���.");
		info->SetPos(Vector2(850.f, 200.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�⺻ �������� ���� �������� �����ϴ�.");
		info->SetPos(Vector2(850.f, 270.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�ΰ��� ��ô");
		info->SetPos(Vector2(175.f, 95.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"6��");
		info->SetPos(Vector2(170.f, 120.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(false);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Ӹ��� ��ü");
		info->SetPos(Vector2(485.f, 95.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"3��");
		info->SetPos(Vector2(485.f, 120.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(false);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"�ڽ��� �Ӹ��� ���� ");
		info->SetPos(Vector2(155.f, 160.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���� �������� �����ϴ�.");
		info->SetPos(Vector2(170.f, 180.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���� �Ӹ��� ȸ���ϸ�");
		info->SetPos(Vector2(160.f, 200.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"��Ÿ���� �ʱ�ȭ�˴ϴ�.");
		info->SetPos(Vector2(167.f, 220.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Ӹ��� ���� ������ ��,");
		info->SetPos(Vector2(470.f, 170.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Ӹ��� �̵��մϴ�.");
		info->SetPos(Vector2(458.f, 190.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);
	}
	else if (SKUL::BERSERKER == m_skulType)
	{
		PanelUI* portrait = new PanelUI;
		portrait->CreateTexture(L"BerserkerSkulPortrait", L"Texture\\UI\\BerserkerSkulIcon.bmp");
		width = (float)portrait->GetTexture()->GetWidth();
		height = (float)portrait->GetTexture()->GetHeight();
		portrait->SetScale(Vector2(width, height));
		portrait->SetPos(Vector2(727.f, -5.f));
		portrait->IsBitBlt(false);
		portrait->SetLook(false);
		portrait->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		portrait->SetSubjectType(SUBJECT_TYPE::SKUL);
		portrait->SetName(L"SkulInfo");
		portrait->SetSkulType(m_skulType);
		_parentUI->GetParentUI()->AddChild(portrait);

		PanelUI* skil = new PanelUI;
		skil->CreateTexture(L"BoneCryIcon", L"Texture\\UI\\BoneCryIcon.bmp");
		width = (float)skil->GetTexture()->GetWidth();
		height = (float)skil->GetTexture()->GetHeight();
		skil->SetScale(Vector2(width, height));
		skil->IsBitBlt(true);
		skil->SetSubjectType(SUBJECT_TYPE::SKUL);
		skil->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		skil->SetPos(Vector2(145.f, 37.f));
		skil->SetSkulType(m_skulType);
		_parentUI->AddChild(skil);

		skil = skil->Clone();
		skil->CreateTexture(L"MadAxeIcon", L"Texture\\UI\\MadAxeIcon.bmp");
		width = (float)skil->GetTexture()->GetWidth();
		height = (float)skil->GetTexture()->GetHeight();
		skil->SetSubjectType(SUBJECT_TYPE::SKUL);
		skil->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
		skil->SetScale(Vector2(width, height));
		skil->SetPos(Vector2(462.f, 37.f));
		skil->SetSkulType(m_skulType);
		_parentUI->AddChild(skil);

		TextUI* info = new TextUI;
		info->SetText(L"����Ŀ");
		info->SetPos(Vector2(850.f, 160.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetSkulType(m_skulType);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"����ũ");
		info->SetPos(Vector2(600.f, 170.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�Ŀ�");
		info->SetPos(Vector2(1085.f, 170.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"Ÿ�� ����� ����� �������� ������ ����.");
		info->SetPos(Vector2(850.f, 200.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�� ������ ��ų '���� ����'�� �ݵ�� �����ϴ�.");
		info->SetPos(Vector2(850.f, 250.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"�ǰ� �� 3�ʰ� ���ݼӵ��� 50% �����մϴ�.");
		info->SetPos(Vector2(850.f, 270.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"���Ӵ뽬 �Ұ�");
		info->SetPos(Vector2(850.f, 290.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(false);
		info->IsBlack(true);
		_parentUI->GetParentUI()->AddChild(info);

		info = info->Clone();
		info->SetText(L"���� ����");
		info->SetPos(Vector2(175.f, 95.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"30��");
		info->SetPos(Vector2(170.f, 120.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(false);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"�ŵ�׽�");
		info->SetPos(Vector2(485.f, 95.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"14��");
		info->SetPos(Vector2(485.f, 120.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(false);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"10�� ���� ���ݼӵ� +100%�� �˴ϴ�.");
		info->SetPos(Vector2(160.f, 160.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"���� ������ ������");
		info->SetPos(Vector2(444.f, 170.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);

		info = info->Clone();
		info->SetText(L"������� ������ ���մϴ�.");
		info->SetPos(Vector2(470.f, 190.f));
		info->SetName(L"SkulInfo");
		info->SetSubjectType(SUBJECT_TYPE::SKUL);
		info->SetSkulType(m_skulType);
		info->SetUIType(UI_TYPE::ITEM_INFO_TEXT);
		info->SetLook(true);
		info->IsBlack(true);
		_parentUI->AddChild(info);
	}

	m_bCreate = true;
}
