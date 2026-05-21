#include "QuickSlotManager.h"
#include "ItemPacketHandler.h"
#include "InventoryManager.h"
#include "CombatPacketHandler.h"
#include "PlayerManager.h"
#include "stbInput.h"
#include "QuickSlotPacketHandler.h"

#define M_INVENTORYMANAGER stb::SingletonBase<InventoryManager>::getInstance()
#define M_PLAYERMANAGER stb::SingletonBase<PlayerManager>::getInstance()
#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()

void QuickSlotManager::Init()
{
    m_slots.clear();
    m_slots.resize(m_maxSlotCount);

    for (int i = 0; i < m_maxSlotCount; ++i)
    {
        m_slots[i].slot_index = i;
    }
}

void QuickSlotManager::Update()
{
 
}

void QuickSlotManager::SetSlot(QuickSlotData& quickSlotData)
{
    if (quickSlotData.slot_index < 0 || quickSlotData.slot_index >= (int)m_slots.size())
        return;

    m_slots[quickSlotData.slot_index] = quickSlotData;
}

void QuickSlotManager::SetSlotSkill(int slotIndex, int skillId)
{

}

void QuickSlotManager::SetSlotItem(int slotIndex, int inventoryType, int slotPos)
{

}

// Äü½½·Ô º¯°æ ÇÏ´Â ¿äÃ» ¼­¹ö·Î º¸³¿
void QuickSlotManager::RequestSetSlot(const QuickSlotData& quickSlotData)
{
    if (quickSlotData.slot_index < 0 || quickSlotData.slot_index >= m_maxSlotCount)
        return;
    QuickSlotPacketHandler::SendSetQuickSlot(quickSlotData);

}

void QuickSlotManager::ClearSlot(int slotIndex)
{

}

const QuickSlotData* QuickSlotManager::GetSlot(int slotIndex) const
{
	return nullptr;
}

void QuickSlotManager::UseSlot(int slotIndex)
{
    const QuickSlotData& slot = m_slots[slotIndex];

    switch (slot.type)
    {
    case QuickSlotType::Skill:
    {
        stb::Player* player = M_PLAYERMANAGER->GetLocalPlayer();
        if (player == nullptr)
            return;

        // auto dir = player->GetFacing();

         //CombatPacketHandler::SendUseSkill(slot.ref_id, static_cast<int>(dir));
        break;
    }
    case QuickSlotType::Item:
    {
        Inventory* inven = M_INVENTORYMANAGER->GetInventory(static_cast<int>(slot.inventory_type));

        InventoryItemInfo* itemInfo = inven->FindSlot(slot.inventory_slotPos);
        if (itemInfo == nullptr)
            return;

        ItemPacketHandler::SendUseItem(itemInfo);
        break;
    }
    default:
        break;
    }
}



