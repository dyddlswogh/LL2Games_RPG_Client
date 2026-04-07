#pragma once
#include "CommonInclude.h"

class Item
{
public:
	Item() = default;
	~Item() = default;

	bool GetIsUseAble() const { return m_useAble; }
	int GetId() const { return m_id; }
	int GetInventoryType() const { return m_inventoryType; }
	int GetMaxCount() const { return m_maxCount; }
	int GetHpRestore() const { return m_hp_restore; }
	int GetMpRestore() const { return m_mp_restore; }
private:
	bool m_useAble = false;
	int m_maxCount = 0;
	int m_id = 0;
	int m_inventoryType = 0;
	int m_hp_restore = 0;
	int m_mp_restore = 0;
};

