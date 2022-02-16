#pragma once
#include "GUI.h"

class InspectorInfoGUI :
    public GUI
{
private :
    ImVec2 m_size;

public :
    void SetSize(ImVec2 _size) { m_size = _size; }
    ImVec2 GetSize() { return m_size; }

    virtual void update() = 0;

protected :
    virtual bool Start() = 0;
    virtual void End() = 0;

public :
    InspectorInfoGUI();
    ~InspectorInfoGUI();
};

