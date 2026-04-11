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
        const char* data = pkt.payload.c_str();
        size_t payloadSize = pkt.payload.size();
        std::string errMsg;

        int metaInfoSize = 0;
        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, metaInfoSize, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        auto inventoryManager = InventoryManager::getInstance();

        for (int i = 0; i < metaInfoSize; i++)
        {
            InventoryMetaInfo inventoryMetaInfo;

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, inventoryMetaInfo.inventoryType, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, inventoryMetaInfo.max_slots, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, inventoryMetaInfo.currnet_slots_size, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

            if (!inventoryManager->CreateInventory(inventoryMetaInfo))
            {
                throw std::runtime_error("CreateInventory Failed");
            }
        }
        OutputDebugStringA("Inventory Create Sucess\n");
    }
    catch (const std::exception& e)
    {
        OutputDebugStringA(e.what());
        OutputDebugStringA("\n");
    }
    catch (...)
    {
        OutputDebugStringA("예상치 못한 에러 발생\n");
    }
}

void InventoryPacketHandler::HandleInventoryItemInfo(const ParsedPacket& pkt)
{
    /*
     payload.push_back(std::to_string(inventory->GetAllItemInfos().size()));

    for(auto itemInfos : inventory->GetAllItemInfos())
    {
        payload.push_back(std::to_string(itemInfos.inventoryType));
        payload.push_back(std::to_string(itemInfos.itemId));
        payload.push_back(std::to_string(itemInfos.itemCount));
        payload.push_back(std::to_string(itemInfos.slotPos));
    }
    */
    try
    {
        size_t offset = 0;
        const char* data = pkt.payload.c_str();
        size_t payloadSize = pkt.payload.size();
        std::string errMsg;
        int metaInfoSize = 0;
        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, metaInfoSize, errMsg))
        {
            throw std::runtime_error(errMsg);
        }
   
        auto inventoryManager = InventoryManager::getInstance();
 
        for (int i = 0; i < metaInfoSize; i++)
        {
            InventoryItemInfo ItemInfo;

            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, ItemInfo.inventoryType, errMsg))
            {
                throw std::runtime_error(errMsg);
            }

  
            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, ItemInfo.itemId, errMsg))
            {
                throw std::runtime_error(errMsg);
            }


            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, ItemInfo.itemCount, errMsg))
            {
                throw std::runtime_error(errMsg);
            }


            if (!PacketParser::ParseNextIntField(data, payloadSize, offset, ItemInfo.slotPos, errMsg))
            {
                throw std::runtime_error(errMsg);
            }
            auto inventory = inventoryManager->GetInventory(ItemInfo.inventoryType);
            if (!inventory->SetSlot(ItemInfo))
            {
                throw std::runtime_error("Inventory SetSlot Failed");
            }
        }
        OutputDebugStringA("Item register Sucess\n");
    }
    catch (const std::exception& e)
    {
        OutputDebugStringA(e.what());
        OutputDebugStringA("\n");
    }
    catch (...)
    {
        OutputDebugStringA("예상치 못한 에러 발생\n");
    }
}

