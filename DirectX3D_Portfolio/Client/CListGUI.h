#pragma once
#include "CGUI.h"

typedef void (CGUI::* GUI_CALLBACK)(DWORD_PTR, DWORD_PTR);
typedef void (*GLOBAL_CALLBACK) (DWORD_PTR, DWORD_PTR);

class CListGUI :
    public CGUI
{
private :
    string m_caption;
    bool m_bPopUp;
    vector<string> m_vecListName;

    GUI_CALLBACK m_doubleClickedCallback;
    CGUI* m_inst;

    GLOBAL_CALLBACK m_doubleClicked;

public :
    void SetCaption(const string& _caption) { m_caption = _caption; }
    void SetList(const vector<wstring>& _vector);
    void SetList(const vector<string>& _vector);
    void SetDoubleClickedCallback(CGUI* _gui, GUI_CALLBACK _func)
    {
        m_inst = _gui;
        m_doubleClickedCallback = _func;
    }

    void SetDoubleClicked(GLOBAL_CALLBACK _func)
    {
        m_doubleClicked = _func;
    }

    void Clear();

    virtual void update();
    virtual void Activate();

public :
    CListGUI();
    virtual ~CListGUI();
};

