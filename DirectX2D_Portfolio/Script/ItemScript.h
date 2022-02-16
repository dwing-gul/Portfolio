#pragma once
#include "UIScript.h"

enum class ITEM_TYPE
{
    TORCH,
    END
};

class ItemScript :
    public UIScript
{
private :
    ITEM_TYPE   m_itemType;
    bool        m_bUse;
    bool        m_bZero;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetItemType(ITEM_TYPE _type) { m_itemType = _type; }
    ITEM_TYPE GetItemType() { return m_itemType; }

    void SetUse(bool _use) { m_bUse = _use; }
    void SetZero(bool _zero) { m_bZero = _zero; }
    bool IsUse() { return m_bUse; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(ItemScript)

public:
    ItemScript();
    ~ItemScript();
};

