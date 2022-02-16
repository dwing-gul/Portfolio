#pragma once
#include "GUI.h"

typedef void (GUI::*GUI_CALLBACK)(DWORD_PTR, DWORD_PTR);

class ListGUI :
    public GUI
{
private :
    string          m_caption;
    bool            m_bPopUp;
    vector<string>  m_vectorListName;
    GUI_CALLBACK    m_doubleClickedCallBack;
    GUI*            m_inst;

public :
    void SetCaption(const string& _caption) { m_caption = _caption; }
    void SetList(const vector<wstring>& _vector);
    void SetList(const vector<string>& _vector);
    void SetDoubleClickedCallBack(GUI* _gui, GUI_CALLBACK _func)
    {
        m_inst = _gui;
        m_doubleClickedCallBack = _func;
    }

    void Clear();

    virtual void update();
    virtual void Activate();

public :
    ListGUI();
    virtual ~ListGUI();
};

