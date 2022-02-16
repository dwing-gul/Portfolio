#pragma once
#include "CUIScript.h"

class CItemBoxUIScript :
    public CUIScript
{
private :
    static UINT g_nextItemID;
    static UINT g_nextBoxID;

private :
    ITEM m_Item; // �ش� �ڽ��� �����ϰ� ���� ������ ����
    EQUIP_TYPE m_equipType;
    Vec3 m_itemPos; // �������� ��ġ�ϴ� ��ǥ
    bool m_bChange; // ������ ������ �ٲ���°�
    bool m_bEquip; // ��� â�� ���� ������ �ڽ��ΰ�?
    bool m_bSlot; // ������ �����ΰ�?
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

