#include "ItemPacketHandler.h"
#include "PacketParser.h"
#include "stbNetworkManager.h"
#include "PlayerManager.h"
#include "CombatPacketHandler.h"

#define M_PLMANAGER stb::SingletonBase<PlayerManager>::getInstance()

void ItemPacketHandler::Execute(const ParsedPacket& pkt)
{

}

void ItemPacketHandler::HandleUseItemResult(const ParsedPacket& pkt)
{
  
    try
    {
        size_t offset = 0;
        const char* data = pkt.payload.c_str();
        size_t payloadSize = pkt.payload.size();
        std::string errMsg;

        UseItemResult useItemResult = {};

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.result, errMsg))
        {
            throw std::runtime_error(errMsg);
        }


        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.errcode, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        if (useItemResult.result == 0)
        {
            // 아이템 사용에 실페했기 떄문에 에러코드를 플레이어 UI에 보내줘야 한다.
            return;
        }

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.inventoryType, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.slotPos, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.item_id, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.used_count, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.remain_count, errMsg))
        {
            throw std::runtime_error(errMsg);
        }


        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.hp, errMsg))
        {
            throw std::runtime_error(errMsg);
        }


        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, useItemResult.mp, errMsg))
        {
            throw std::runtime_error(errMsg);
        }

       auto localPlayer = M_PLMANAGER->GetLocalPlayer();
       if (localPlayer == nullptr)
       {
           throw std::runtime_error("localPlayer is nullptr");
       }
       auto inventoryManager = localPlayer->GetInvenManager();
       if (inventoryManager == nullptr)
       {
           throw std::runtime_error("inventoryManager is nullptr");
       }

       auto localPlayerStat = localPlayer->GetStat();
       if (localPlayerStat == nullptr)
       {
           throw std::runtime_error("localPlayerStat is nullptr");
       }

       auto inventory = inventoryManager->GetInventory(useItemResult.inventoryType);
       if (inventory == nullptr)
       {
           throw std::runtime_error("inventory is nullptr");
       }

       auto item = inventory->FindSlot(useItemResult.slotPos);
       if (item == nullptr)
       {
           throw std::runtime_error("item is nullptr");
       }

       if (item->itemId != useItemResult.item_id)
       {
           throw std::runtime_error("item_id mismatch");
       }

       if (item->itemCount < useItemResult.used_count)
       {
           throw std::runtime_error("used_count is bigger than current itemCount");
       }

       if (useItemResult.remain_count <= 0)
       {
           if (!inventory->RemoveItem(useItemResult.slotPos))
           {
               throw std::runtime_error("RemoveItem failed");
           }
       }
       else
       {
           item->itemCount = useItemResult.remain_count;
       }

       localPlayerStat->SetCurHp(useItemResult.hp);
       localPlayerStat->SetCurMp(useItemResult.mp);

    }
    catch (std::exception& e)
    {
        OutputDebugStringA(e.what());
        OutputDebugStringA("\n");
    }
    catch(...)
    {
        OutputDebugStringA("예상치 못한 에러입니다.\n\n");
    }
}


void ItemPacketHandler::SendUseItem(InventoryItemInfo* inventoryitemInfo)
{
    std::vector<std::string> data;

    data.push_back(std::to_string(inventoryitemInfo->inventoryType));
    data.push_back(std::to_string(inventoryitemInfo->slotPos));
    data.push_back(std::to_string(inventoryitemInfo->itemId));
    data.push_back(std::to_string(inventoryitemInfo->itemCount));


    // 패킷 생성 및 전송
    std::string body = PacketParser::MakeBody(data);
    std::string packet = PacketParser::MakePacket(PKT_PLAYER_USE_ITEM, body);

    stb::NetworkManager::getInstance()->SendPacket(PKT_PLAYER_USE_ITEM, data);
    OutputDebugStringA("[PKT_PLAYER_USE_ITEM 전송 완료]\n\n");
}