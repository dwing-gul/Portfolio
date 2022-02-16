#include "stdafx.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "UI.h"
#include "KeyManager.h"

UIManager::UIManager() :
    m_focusedUI(nullptr)
{
}

UIManager::~UIManager()
{
}

void UIManager::update()
{
    UI* prevUI = m_focusedUI;
    m_focusedUI = GetFocusedUI();

    if (prevUI != m_focusedUI)
    {
        Scene* curScene = SceneManager::GetInst()->GetCurScene();
        vector<Obj*>& vectorUI = curScene->GetObjects(GROUP_TYPE::UI);

        vector<Obj*>::iterator iter = vectorUI.begin();
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

void UIManager::MoveFocusedUI(UI* _targetUI)
{
    // UI 벡터에서 대상 찾아서 삭제
    vector<Obj*>& vectorUI = SceneManager::GetInst()->GetCurScene()->GetObjects(GROUP_TYPE::UI);
    vector<Obj*>::iterator iter = vectorUI.begin();
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

void UIManager::UIEvent(UI* _parentUI)
{
    KEY_STATE lButtonState = KeyManager::GetInst()->GetState(KEY::LBUTTON);
    KEY_STATE rButtonState = KeyManager::GetInst()->GetState(KEY::RBUTTON);

    UI* ui = GetTargetedUI(_parentUI);

    if (ui)
    {
        if (KEY_STATE::TAP == lButtonState)
        {
            ui->LButtonDown(MOUSE_POS);
            m_focusedUI = _parentUI;
        }
        else if (KEY_STATE::AWAY == lButtonState)
        {
            ui->LButtonUp(MOUSE_POS);

            if (ui->m_bLButtonDown)
            {
                ui->LButtonClick(MOUSE_POS);
                ui->m_bLButtonDown = false;
            }
        }

        if (KEY_STATE::TAP == rButtonState)
        {
            ui->RButtonDown(MOUSE_POS);
            m_focusedUI = _parentUI;
        }
        else if (KEY_STATE::AWAY == rButtonState)
        {
            ui->RButtonUp(MOUSE_POS);

            if (ui->m_bRButtonDown)
            {
                ui->RButtonClick(MOUSE_POS);
                ui->m_bRButtonDown = false;
            }
        }
    }
}

UI* UIManager::GetFocusedUI()
{
    KEY_STATE lButtonState = KeyManager::GetInst()->GetState(KEY::LBUTTON);

    Scene* curScene = SceneManager::GetInst()->GetCurScene();

    if (nullptr == curScene)
        return nullptr;

    vector<Obj*>& vectorUI = curScene->GetObjects(GROUP_TYPE::UI);

    UI* focusedUI = m_focusedUI;

    for (size_t i = 0; i < vectorUI.size(); ++i)
    {
        if (((UI*)vectorUI[i])->IsMouseOn(MOUSE_POS) && lButtonState == KEY_STATE::TAP)
        {
            focusedUI = ((UI*)vectorUI[i]);
        }
    }

    return focusedUI;
}

UI* UIManager::GetTargetedUI(UI* _parentUI)
{
    KEY_STATE lButtonState = KeyManager::GetInst()->GetState(KEY::LBUTTON);

    list<UI*> queue;

    queue.push_back(_parentUI);
    UI* target = nullptr;

    while (!queue.empty())
    {
        UI* ui = queue.front();
        queue.pop_front();

        const vector<UI*>& vectorChild = ui->GetChild();
        for (size_t i = 0; i < vectorChild.size(); ++i)
        {
            queue.push_back(vectorChild[i]);
        }

        if (ui->IsMouseOn(MOUSE_POS))
        {
            ui->m_bMouseOn = true;

            if (nullptr != target)
            {
                target->m_bMouseOn = false;
            }

            target = ui;
        }
        else
        {
            ui->m_bMouseOn = false;
        }
    }

    return target;
}
