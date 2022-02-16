#pragma once
#include <Engine/Script.h>
#include "ItemScript.h"

class ItemManagerScript :
    public Script
{
private :
    static int m_arrItemCount[(UINT)ITEM_TYPE::END];

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void AddItem(ITEM_TYPE _type) { m_arrItemCount[(UINT)_type] += 1; }

    int GetItemCount(ITEM_TYPE _type) { return m_arrItemCount[(UINT)_type]; }
    CLONE(ItemManagerScript)

public:
    ItemManagerScript();
    ~ItemManagerScript();
};

