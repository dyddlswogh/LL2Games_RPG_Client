#include "Inventory.h"

Inventory::Inventory(InventoryMetaInfo inventoryMetaInfo)
{
	m_inventoryType = inventoryMetaInfo.inventoryType;
	m_maxSlot = inventoryMetaInfo.max_slots;
	m_current_slot_size = inventoryMetaInfo.currnet_slots_size;

	m_slots.reserve(m_maxSlot);

	for (int i = 0; i < m_maxSlot; i++)
	{
		InventoryItemInfo info{};
		info.inventoryType = m_inventoryType;
		info.slotPos = i;
		info.itemId = 0;
		info.itemCount = 0;

		m_slots.emplace(i, info);
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

	if (itemInfo.itemId == 0)
	{
		return true; // ºó ½½·ÔÀº ¹«½Ã
	}

	auto it = m_slots.find(itemInfo.slotPos);

	if (it == m_slots.end())
	{
		OutputDebugStringA("SetSlot find fail\n");
		return false;
	}

	it->second.inventoryType = m_inventoryType;
	it->second.slotPos = itemInfo.slotPos;
	it->second.itemId = itemInfo.itemId;
	it->second.itemCount = itemInfo.itemCount;
	if (itemInfo.itemId != 0)
	{
		
		std::string msg;
		msg = "[m_slot Pos] =" + std::to_string(itemInfo.slotPos)
			+ " [SetSlot After] slotPos=" + std::to_string(it->second.slotPos)
			+ " itemId=" + std::to_string(it->second.itemId)
			+ " itemCount=" + std::to_string(it->second.itemCount) + "\n";
		OutputDebugStringA(msg.c_str());

	}

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



std::vector<InventoryItemInfo> Inventory::GetItemInfos()
{
	std::vector<InventoryItemInfo> items;
	std::string msg;
	msg = "[SetSlot] this=" + std::to_string((uintptr_t)this) + "\n";
	OutputDebugStringA(msg.c_str());

	for (auto& [pos, item] : m_slots)
	{
		if (item.itemId != 0 && item.itemCount > 0)
		{
			std::string DebugMsg = "Item ID [" + std::to_string(item.itemId) +"]" + "\n";
			OutputDebugStringA(DebugMsg.c_str());
			DebugMsg = "Item SlotPos [" + std::to_string(item.slotPos) + "]" + "\n";
			OutputDebugStringA(DebugMsg.c_str());
			items.push_back(item);
		}
	}
	return items;
}
