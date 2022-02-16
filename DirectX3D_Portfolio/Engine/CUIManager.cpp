#include "pch.h"
#include "CUIManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CGameObject.h"
#include "CCollider2D.h"

CUIManager::CUIManager() :
    m_focusedUI(nullptr)
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::update()
{
    CGameObject* prevUI = m_focusedUI;
    m_focusedUI = GetFocusedUI();

    if (nullptr == m_focusedUI)
        return;

    if (prevUI != m_focusedUI)
    {
        vector<CGameObject*>& vectorUI = CSceneManager::GetInst()->FindObjByType_Ref(OBJECT_TYPE::UI);

        vector<CGameObject*>::iterator iter = vectorUI.begin();
        for (; iter != vectorUI.end(); ++iter)
        {
            if ((*iter) == m_focusedUI)
            {
                vectorUI.erase(iter);
                break;
            }
        }

        vectorUI.push_back(m_focusedUI);
    }

    if (nullptr != m_focusedUI)
    {
        if (m_focusedUI->IsDead())
        {
            m_focusedUI = nullptr;
        }
        else
            UIEvent(m_focusedUI);
    }
}

void CUIManager::MoveFocusedUI(CGameObject* _targetUI)
{
    // UI 벡터에서 대상 찾아서 삭제
    vector<CGameObject*>& vectorUI = CSceneManager::GetInst()->FindObjByType_Ref(OBJECT_TYPE::UI, true);
    vector<CGameObject*>::iterator iter = vectorUI.begin();
    for (size_t i = 0; iter != vectorUI.end() && i < vectorUI.size();)
    {
        if (vectorUI[i]->GetName() == _targetUI->GetName())
        {
            iter = vectorUI.erase(iter);
        }
        else
        {
            ++iter;
            ++i;
        }
    }

    // 벡터의 뒤에 해당 UI 추가
    vectorUI.push_back(_targetUI);
    // 포커싱 중인 UI 변경
    m_focusedUI = _targetUI;
}

void CUIManager::UIEvent(CGameObject* _parentUI)
{
    CGameObject* ui = GetTargetedUI(_parentUI);

    if (ui)
    {
        if (ui->Collider2D()->IsObjectHovered() && IsMouseClicked())
        {
            m_focusedUI = _parentUI;
        }

        if (ui->Collider2D()->IsObjectHovered() && IsMouseClicked(MOUSE_BUTTON::RIGHT))
        {
            m_focusedUI = _parentUI;
        }
    }
}

CGameObject* CUIManager::GetFocusedUI()
{
    vector<CGameObject*>& vectorUI = CSceneManager::GetInst()->FindObjByType_Ref(OBJECT_TYPE::UI, true);

    CGameObject* focusedUI = nullptr;

    for (size_t i = 0; i < vectorUI.size(); ++i)
    {
        if (vectorUI[i]->Collider2D()->IsObjectHovered() && IsMouseClicked())
        {
            focusedUI = vectorUI[i];
            m_bFocused = true;
        }
        else
        {
            m_bFocused = false;
        }
    }

    return focusedUI;
}

CGameObject* CUIManager::GetTargetedUI(CGameObject* _parentUI)
{
    list<CGameObject*> queue;

    queue.push_back(_parentUI);
    CGameObject* target = nullptr;

    while (!queue.empty())
    {
        CGameObject* ui = queue.front();
        queue.pop_front();

        const vector<CGameObject*>& vectorChild = ui->GetChilds();
        for (size_t i = 0; i < vectorChild.size(); ++i)
        {
            queue.push_back(vectorChild[i]);
        }

        if (ui->Collider2D()->IsObjectHovered())
        {
            target = ui;
        }
    }

    return target;
}
