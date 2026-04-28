#pragma once
#include "CommonInclude.h"
#include "stbInput.h"


enum class QuickSlotType
{
    None,
    Skill,
    Item
};

struct QuickSlotData
{
    QuickSlotType type = QuickSlotType::None;
    int slot_index = 0;
    int ref_id = 0;
    int inventory_type = 0;
    int inventory_slotPos = 0;
};


class QuickSlotManager
{
public:
    void Init();
    void Update();

    void ClearSlot(int slotIndex);
    void UseSlot(int slotIndex);
public:
    void SetSlot(QuickSlotData& quickSlotData);
    void SetSlotSkill(int slotIndex, int skillId);
    void SetSlotItem(int slotIndex, int inventoryType, int slotPos);

    const QuickSlotData* GetSlot(int slotIndex) const;
   

public:
   


private:
    std::vector<QuickSlotData> m_slots;

private:
    static constexpr int m_maxSlotCount = 32;

};


namespace QuickSlot
{
    inline QuickSlotType SetQuickSlotType(int type)
    {
        switch (type)
        {
        case 1: return QuickSlotType::Skill;
        case 2: return QuickSlotType::Item;
        default: return QuickSlotType::None;
        }
    }
}
