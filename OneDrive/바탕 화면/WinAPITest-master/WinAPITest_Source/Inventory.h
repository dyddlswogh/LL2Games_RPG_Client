#pragma once
#include "CommonInclude.h"
#include "Item.h"
#include "Inventory_Info.h"

struct InventorySlot
{
	int inventoryType = 0;
	int slotPos = 0;
	int itemId = 0;
	int itemCount = 0;
	bool isEnable = false;
};

class Inventory
{
public:
	Inventory() = default;
	Inventory(InventoryMetaInfo inventoryMetaInfo);
	InventorySlot* FindSlot(int slotPos);
	bool SetSlot(int slotPos, int itemId, int itemCount);
	bool RemoveItem(int slotPos, int count);

	void Clear();
private:
	int m_inventoryType;
	int m_maxSlot;
	int m_current_slot_size;
	std::unordered_map<int, InventorySlot> m_slots;
};

