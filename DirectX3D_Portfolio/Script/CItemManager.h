#pragma once

class CItemManager :
	public CSingleton<CItemManager>
{
	SINGLE(CItemManager);
private :
	tItem m_arrItem[(UINT)ITEM::END];
	tItem m_endItem;

public :
	void init();

	ITEM ItemCombination(ITEM _left, ITEM _right);
	tItem& GetItemInfo(ITEM _item) 
	{ 
		if (ITEM::END == _item)
		{
			return m_endItem;
		}

		return m_arrItem[(UINT)_item]; 
	}
};

