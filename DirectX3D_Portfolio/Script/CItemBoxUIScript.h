#pragma once
#include "CUIScript.h"

class CItemBoxUIScript :
    public CUIScript
{
private :
    static UINT g_nextItemID;
    static UINT g_nextBoxID;

private :
    ITEM m_Item; // 해당 박스를 차지하고 있을 아이템 정보
    EQUIP_TYPE m_equipType;
    Vec3 m_itemPos; // 아이템이 위치하는 좌표
    bool m_bChange; // 아이템 정보가 바뀌었는가
    bool m_bEquip; // 장비 창에 대한 아이템 박스인가?
    bool m_bSlot; // 아이템 슬롯인가?
    bool m_bBox;
    UINT m_itemID;
    UINT m_boxID;

    vector<CGameObject*>        m_vecBox;
    CGameObject*                m_SkillPanelUI;
    CGameObject*                m_InventoryPanelUI;
    CGameObject*                m_ProfilePanelUI;
    CGameObject*                m_PlayerStatPanelUI;
    CGameObject*                m_ItemBoxPanelUI;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    ITEM GetItem() { return m_Item; }
    void SetItem(ITEM _item) { m_Item = _item; m_bChange = true; }
    void SetEquip(bool _equip, EQUIP_TYPE _type) { m_bEquip = true; m_equipType = _type; }
    void SetSlot(bool _slot) { m_bSlot = _slot; }
    bool IsEquip() { return m_bEquip; }

    virtual void PlayerChange();
 
private :
    bool CanEquipCheck(CItemBoxUIScript* _box, tItem& _itemInfo);
    void CheckEquip();
    void CheckInventory();
    void CheckBox();
    void SetImage();

    CLONE(CItemBoxUIScript)

public:
    CItemBoxUIScript();
    ~CItemBoxUIScript();
};

