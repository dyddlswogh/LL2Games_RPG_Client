#include "InventoryPacketHandler.h"
#include "PacketParser.h"
#include "StringConvert.h"
#include "InventoryManager.h"

void InventoryPacketHandler::Execute(const ParsedPacket& pkt)
{
	(void)pkt;
}

void InventoryPacketHandler::MoveInventoryItem()
{

}

void InventoryPacketHandler::HandleInventoryMetaInfo(const ParsedPacket& pkt)
{
	try
	{
		size_t offset = 0;
		size_t payloadSize = pkt.payload.size();
		std::string errMsg;
		std::string debugMsg = "payloadSize :" + payloadSize;
		OutputDebugStringA(debugMsg.c_str());

		if (payloadSize < sizeof(uint16_t))
		{
			//LOG("[??? ???] ????ε? ??? ????\n");
			return;
		}

		std::string str_metaInfoSize;

		// 아이템 메타 정보 개수 
		if (!PacketParser::ParseLengthPrefixedString(
            pkt.payload.c_str(),
            payloadSize,
            offset,
            str_metaInfoSize,
            errMsg
        ))
        {
            // 로그 출력 필요
            return;
        }

		auto inventoryManager = InventoryManager::getInstance();
		int metaInfoSize = 0;

		if(!Convert::StringToInt(str_metaInfoSize, metaInfoSize))
		{
			// 로그 출력 필요
			return;
		}

		for(int i =0; i< metaInfoSize; i++)
		{
			STRInventoryMetaInfo str_inventoryMetaInfo;
			InventoryMetaInfo inventoryMetaInfo;

			if(!ReadIntValue(str_inventoryMetaInfo, inventoryMetaInfo))
			{
				//에러 로그 필요
				return;
			}
			
			if(!inventoryManager->CreateInventory(inventoryMetaInfo))
			{
				//에러 로그 필요
				return;
			}
		}

	}
	catch (...)
	{
		// 에러 로그 필요
	}
}

void InventoryPacketHandler::HandleInventoryItemInfo(const ParsedPacket& pkt)
{
	try
	{

	}
	catch(...)
	{
		// 에러 로그 필요
	}
}

bool InventoryPacketHandler::ReadIntValue(STRInventoryMetaInfo& str_inventoryMetaInfo, InventoryMetaInfo& inventoryMetaInfo)
{
	if(!Convert::StringToInt(str_inventoryMetaInfo.inventoryType, inventoryMetaInfo.inventoryType))
	{
		return false;
	}

	if(!Convert::StringToInt(str_inventoryMetaInfo.max_slots, inventoryMetaInfo.max_slots))
	{
		return false;
	}
	
	if(!Convert::StringToInt(str_inventoryMetaInfo.current_slots_size, inventoryMetaInfo.currnet_slots_size))
	{
		return false;
	}

	return true;
}
