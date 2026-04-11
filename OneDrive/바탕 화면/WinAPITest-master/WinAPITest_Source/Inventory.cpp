#include "Inventory.h"

Inventory::Inventory(InventoryMetaInfo inventoryMetaInfo)
{
	m_inventoryType = inventoryMetaInfo.inventoryType;
	m_maxSlot = inventoryMetaInfo.max_slots;
	m_current_slot_size = inventoryMetaInfo.currnet_slots_size;

	m_slots.reserve(m_maxSlot);

	for (int i = 1; i <= m_maxSlot; i++)
	{
		m_slots[i].slotPos = i;
	}
}


InventoryItemInfo* Inventory::FindSlot(int slotPos)
{
	auto it = m_slots.find(slotPos);

	if (it != m_slots.end())
	{
		return &(it->second);
	}

	return nullptr;
}

bool Inventory::SetSlot(int slotPos, int itemId, int itemCount)
{
	auto it = m_slots.find(slotPos);

	if (it == m_slots.end())
	{
		return false;
	}

	it->second.inventoryType = m_inventoryType;
	it->second.slotPos = slotPos;
	it->second.itemId = itemId;
	it->second.itemCount = itemCount;

	return true;
}

bool Inventory::SetSlot(InventoryItemInfo& itemInfo)
{
	auto it = m_slots.find(itemInfo.slotPos);

	if (it == m_slots.end())
	{
		return false;
	}

	it->second.inventoryType = m_inventoryType;
	it->second.slotPos = itemInfo.slotPos;
	it->second.itemId = itemInfo.itemId;
	it->second.itemCount = itemInfo.itemCount;

	return true;
}


bool Inventory::RemoveItem(int slotPos, int count)
{
	auto it = m_slots.find(slotPos);

	if (it == m_slots.end())
	{
		return false;
	}

	if (it->second.itemCount < count)
	{
		return false;
	}

	it->second.itemCount -= count;

	if (it->second.itemCount <= 0)
	{
		m_slots.erase(it);
	}

	return true;
}

bool Inventory::RemoveItem(int slotPos)
{
	auto it = m_slots.find(slotPos);

	if (it == m_slots.end())
	{
		return false;
	}

	m_slots.erase(it);

	return true;
}
