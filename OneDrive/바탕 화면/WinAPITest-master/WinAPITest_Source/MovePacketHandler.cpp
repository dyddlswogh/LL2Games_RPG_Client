#include "MovePacketHandler.h"
#include "stbNetworkConfig.h"
#include "stbOtherPlayerManager.h"
#include "PacketParser.h"
#include "StringConvert.h"
#include "PacketData.h"

/*
struct ParsedPacket
{
    uint16_t type;
    std::string payload;
};
*/



void MovePacketHandler::Execute(const ParsedPacket& pkt)
{
    try
    {
        size_t offset = 0;
        size_t payloadSize = pkt.payload.size();

        std::string playerId;
        std::string str_xPos;
        std::string str_yPos;
        std::string str_speed;
        std::string errMsg;

        if (payloadSize < sizeof(uint16_t))
        {
            //LOG("[이동 패킷] 페이로드 크기 부족\n");
            return;
        }

        // 1. playerID (char_id)
        if (!PacketParser::ParseLengthPrefixedString(
            pkt.payload.c_str(),
            payloadSize,
            offset,
            playerId,
            errMsg
        ))
        {
            // 로그 출력 필요
            return;
        }

        if (playerId == stb::NetworkConfig::GetCharacterId())
        {
            return;
        }

        //2. xPos
        if (!PacketParser::ParseLengthPrefixedString(
            pkt.payload.c_str(),
            payloadSize,
            offset,
            str_xPos,
            errMsg
        ))
        {
            // 로그 출력 필요
            return;
        }

        //3. yPos
        if (!PacketParser::ParseLengthPrefixedString(
            pkt.payload.c_str(),
            payloadSize,
            offset,
            str_yPos,
            errMsg
        ))
        {
            // 로그 출력 필요
            return;
        }

        //4. speed
        if (!PacketParser::ParseLengthPrefixedString(
            pkt.payload.c_str(),
            payloadSize,
            offset,
            str_speed,
            errMsg
        ))
        {
            // 로그 출력 필요
            return;
        }

        OtherPlayerMove otherPlayerMove{};

        otherPlayerMove.playerId = playerId;

        if (!Convert::StringToFloat(str_xPos, otherPlayerMove.xPos))
        {
            // 로그 출력 필요
            return;
        }

        if (!Convert::StringToFloat(str_yPos, otherPlayerMove.yPos))
        {
            // 로그 출력 필요
            return;
        }

        if (!Convert::StringToFloat(str_speed, otherPlayerMove.speed))
        {
            // 로그 출력 필요
            return;
        }

        auto otherPlayerMgr = stb::OtherPlayerManager::getInstance();

        if (otherPlayerMgr != nullptr)
        { 
            otherPlayerMgr->HandleMovePacket(otherPlayerMove);
        }
        else
        {
            // 로그 출력 필요
            return;
        }
    }
    catch (const std::exception& e)
    {
        std::string msg = "[이동 패킷] 예외 발생: ";
        msg += e.what();
        msg += "\n";
        //LOG(msg);
    }
    catch (...)
    {
        //LOG("[이동 패킷] 알 수 없는 예외 발생\n");
    }
    
}
