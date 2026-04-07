#pragma once

#include "IPacketHandler.h"
#include "Packet.h"
#include "CommonInclude.h" 
#include "Inventory_info.h"

class InventoryPacketHandler : public IPacketHandler
{
public:
	static void Execute(const ParsedPacket& pkt);
	static void HandleInventoryMetaInfo(const ParsedPacket& pkt);
	static void HandleInventoryItemInfo(const ParsedPacket& pkt);
	static bool ReadIntValue(STRInventoryMetaInfo& str_inventoryMetaInfo, InventoryMetaInfo& inventoryMetaInfo);
	static void MoveInventoryItem();

private:

};

