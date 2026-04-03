#include "MovePacketHandler.h"
#include "stbNetworkConfig.h"
#include "stbOtherPlayerManager.h"
#include "PacketParser.h"
#include "StringConvert.h"

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
        const char* data = pkt.payload.data();
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



    }
        /*
        uint16_t len1 = *(uint16_t*)(data + offset);
        offset += sizeof(uint16_t);

        if (offset + len1 > payloadSize)
        {
            //LOG("[이동 패킷] playerID 길이 초과\n");
            return;
        }

        std::string charId(data + offset, len1);
        offset += len1;
        //LOG("[이동 패킷] playerID: " << charId << "\n");

        // 내 캐릭터 ID와 같으면 무시 (자기 자신)
        if (charId == stb::NetworkConfig::GetCharacterId())
        {
            //LOG("[이동 패킷] 내 캐릭터 무시\n");
            return;
        }

        // 2. xPos
        if (offset + sizeof(uint16_t) > payloadSize)
        {
            //LOG("[이동 패킷] xPos 헤더 부족\n");
            return;
        }

        uint16_t len2 = *(uint16_t*)(data + offset);
        offset += sizeof(uint16_t);

        if (offset + len2 > payloadSize)
        {
            //LOG("[이동 패킷] xPos 길이 초과\n");
            return;
        }

        std::string xStr(data + offset, len2);
        offset += len2;
        //LOG("[이동 패킷] xPos: " << xStr << "\n");

        // 3. yPos
        if (offset + sizeof(uint16_t) > payloadSize)
        {
            //LOG("[이동 패킷] yPos 헤더 부족\n");
            return;
        }

        uint16_t len3 = *(uint16_t*)(data + offset);
        offset += sizeof(uint16_t);

        if (offset + len3 > payloadSize)
        {
            //LOG("[이동 패킷] yPos 길이 초과\n");
            return;
        }

        std::string yStr(data + offset, len3);
        offset += len3;
        //LOG("[이동 패킷] yPos: " << yStr << "\n");

        // 4. speed
        if (offset + sizeof(uint16_t) > payloadSize)
        {
            //LOG("[이동 패킷] speed 헤더 부족\n");
            return;
        }

        uint16_t len4 = *(uint16_t*)(data + offset);
        offset += sizeof(uint16_t);

        if (offset + len4 > payloadSize)
        {
            //LOG("[이동 패킷] speed 길이 초과\n");
            return;
        }

        std::string speedStr(data + offset, len4);
        //LOG("[이동 패킷] speed: " << speedStr << "\n");

        float x = (float)atof(xStr.c_str());
        float y = (float)atof(yStr.c_str());
        float speed = (float)atof(speedStr.c_str());

        // LOG("[이동 성공!] Char " << charId << " -> (" << x << ", " << y << ") speed: " << speed << "\n");

        // 다른 플레이어 위치 업데이트
        auto otherPlayerMgr = stb::OtherPlayerManager::getInstance();
        if (otherPlayerMgr != nullptr)
        {
            // LOG("[이동 패킷] OtherPlayerManager 업데이트 시작\n");
            // SetTargetPosition 사용 (부드러운 이동)
            auto it = otherPlayerMgr->GetPlayers().find(charId);
            if (it != otherPlayerMgr->GetPlayers().end() && it->second != nullptr)
            {
                it->second->SetTargetPosition(x, y);
            }
            else
            {
                // 처음 보는 플레이어면 생성
                otherPlayerMgr->UpdatePlayer(charId, x, y);
            }
            // LOG("[이동 패킷] OtherPlayerManager 업데이트 완료\n");
        }
        else
        {
           // LOG("[이동 패킷] OtherPlayerManager가 nullptr!\n");
        }
    }
    */
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
