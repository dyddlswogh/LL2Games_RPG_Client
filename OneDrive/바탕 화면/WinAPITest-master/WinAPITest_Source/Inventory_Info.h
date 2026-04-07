#pragma once
#include "CommonInclude.h"

struct STRInventoryMetaInfo
{
    std::string inventoryType;
    std::string max_slots;
    std::string current_slots_size;
};

struct InventoryMetaInfo
{
    int inventoryType = 0;
    int max_slots = 0;
    int currnet_slots_size = 0;
};

struct InventoryItemInfo
{
    int inventoryType = 0;
    int slotPos = 0;
    int itemId = 0;
    int itemCount = 0;
};