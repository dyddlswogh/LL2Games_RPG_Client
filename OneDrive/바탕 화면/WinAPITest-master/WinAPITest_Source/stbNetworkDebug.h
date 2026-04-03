#pragma once
#include "stbNetworkManager.h"
#include "stbNetworkConfig.h"
#include "stbOtherPlayerManager.h"
#include "stbLogger.h"
#include "Packet.h"
#include <string>
#include <sstream>
#include <iomanip>

namespace stb
{
    // 함수 전방 선언
    inline void SendEnterMap(const std::string& charId, const std::string& mapId);

    // 패킷 데이터를 16진수로 출력하는 헬퍼 함수
    inline void PrintPacketHex(const std::string& packet, const char* label)
    {
        std::stringstream ss;
        ss << label << " (" << packet.size() << " bytes): ";
        
        for (size_t i = 0; i < packet.size(); i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') 
               << (int)(unsigned char)packet[i] << " ";
        }
        ss << "\n";
        
        OutputDebugStringA(ss.str().c_str());
    }

    // 디버그 모드로 네트워크 초기화
    inline void InitializeNetworkDebug(HWND hWnd)
    {
        std::stringstream ss;
        ss << "=== 네트워크 디버그 모드 ===\n";
        ss << "서버 IP: " << NetworkConfig::SERVER_IP << "\n";
        ss << "서버 포트: " << NetworkConfig::SERVER_PORT << "\n";
        OutputDebugStringA(ss.str().c_str());

        bool connected = NetworkManager::getInstance()->Connect(
            NetworkConfig::SERVER_IP,
            NetworkConfig::SERVER_PORT,
            hWnd
        );

        if (connected)
        {
            OutputDebugStringA("서버 연결 시도 중...\n");
        }
        else
        {
            OutputDebugStringA("서버 연결 실패! (소켓 생성 또는 connect 실패)\n");
        }
    }

    // 연결 상태 확인
    inline void CheckConnectionStatus()
    {
        if (NetworkManager::getInstance()->IsConnected())
        {
            OutputDebugStringA("현재 상태: 서버 연결됨\n");
        }
        else
        {
            OutputDebugStringA("현재 상태: 서버 연결 안 됨\n");
        }
    }

    // 채널 인증 응답 핸들러 등록
    inline void RegisterChannelAuthHandler()
    {
        NetworkManager::getInstance()->RegisterHandler(PKT_CHANNEL_AUTH,
            [](const ParsedPacket& pkt)
            {
                OutputDebugStringA("채널 인증 응답 받음!\n");
                
                // 응답 데이터 파싱 (필요시)
                try
                {
                    if (pkt.payload.size() > sizeof(uint16_t))
                    {
                        const char* data = pkt.payload.data();
                        size_t offset = 0;

                        uint16_t len = *(uint16_t*)(data + offset);
                        offset += sizeof(uint16_t);

                        if (offset + len <= pkt.payload.size())
                        {
                            std::string response(data + offset, len);
                            std::string msg = "서버 응답: " + response + "\n";
                            OutputDebugStringA(msg.c_str());
                        }
                    }
                    
                    // 채널 인증 성공 후 맵 입장 패킷 전송
                    OutputDebugStringA("채널 인증 완료! 맵 입장 패킷 전송...\n");
                    SendEnterMap(NetworkConfig::GetCharacterId(), NetworkConfig::MAP_ID);
                }
                catch (...)
                {
                    OutputDebugStringA("채널 인증 응답 파싱 중 오류\n");
                }
            }
        );
    }

    // 채널 인증 패킷 전송 (패킷 타입 0x09, 데이터: 캐릭터 ID)
    inline void SendChannelAuth()
    {
        std::string charId = NetworkConfig::GetCharacterId();
        std::vector<std::string> data = { charId };
        
        std::stringstream ss;
        ss << "\n[PKT_CHANNEL_AUTH 전송]\n";
        ss << "  패킷 타입: 0x" << std::hex << PKT_CHANNEL_AUTH << " (" << std::dec << PKT_CHANNEL_AUTH << ")\n";
        ss << "  캐릭터 ID: " << charId << "\n";
        OutputDebugStringA(ss.str().c_str());
        
        // 패킷 생성 및 전송
        std::string body = PacketParser::MakeBody(data);
        std::string packet = PacketParser::MakePacket(PKT_CHANNEL_AUTH, body);
        
        // 패킷 내용 출력
        PrintPacketHex(packet, "  전송 패킷");
        
        NetworkManager::getInstance()->SendPacket(PKT_CHANNEL_AUTH, data);
        OutputDebugStringA("[PKT_CHANNEL_AUTH 전송 완료]\n\n");
    }

    // 맵 입장 패킷 전송 (캐릭터 ID, 맵 ID)
    inline void SendEnterMap(const std::string& charId, const std::string& mapId)
    {
        std::vector<std::string> data = { charId, mapId };
        
        std::stringstream ss;
        ss << "\n[PKT_ENTER_MAP 전송]\n";
        ss << "  패킷 타입: 0x" << std::hex << PKT_ENTER_MAP << " (" << std::dec << PKT_ENTER_MAP << ")\n";
        ss << "  캐릭터 ID: " << charId << "\n";
        ss << "  맵 ID: " << mapId << "\n";
        OutputDebugStringA(ss.str().c_str());
        
        // 패킷 생성 및 전송
        std::string body = PacketParser::MakeBody(data);
        std::string packet = PacketParser::MakePacket(PKT_ENTER_MAP, body);
        
        // 패킷 내용 출력
        PrintPacketHex(packet, "  전송 패킷");
        
        NetworkManager::getInstance()->SendPacket(PKT_ENTER_MAP, data);
        OutputDebugStringA("[PKT_ENTER_MAP 전송 완료]\n\n");
    }

    // 플레이어 이동 패킷 전송 (x, y, speed)
    inline void SendPlayerMove(float x, float y, float speed)
    {
        if (!NetworkManager::getInstance()->IsConnected())
        {
            return;  // 연결 안 되어 있으면 전송 안 함
        }

        char xStr[32], yStr[32], speedStr[32];
        sprintf_s(xStr, "%.1f", x);
        sprintf_s(yStr, "%.1f", y);
        sprintf_s(speedStr, "%.1f", speed);
        
        std::vector<std::string> data = { xStr, yStr, speedStr };
        NetworkManager::getInstance()->SendPacket(PKT_PLAYER_MOVE, data);
        
        // 디버그 로그 (너무 많이 나올 수 있으니 주석 처리 가능)
        // std::stringstream ss;
        // ss << "[전송] 이동 패킷: (" << x << ", " << y << ")\n";
        // OutputDebugStringA(ss.str().c_str());
    }

    // 맵 입장 응답 핸들러
    inline void RegisterEnterMapHandler()
    {
        NetworkManager::getInstance()->RegisterHandler(PKT_ENTER_MAP,
            [](const ParsedPacket& pkt)
            {
                try
                {
                    if (pkt.payload.size() < sizeof(uint16_t))
                    {
                        OutputDebugStringA("[맵 입장] 페이로드 크기 부족\n");
                        return;
                    }

                    const char* data = pkt.payload.data();
                    size_t offset = 0;

                    // status
                    uint16_t len1 = *(uint16_t*)(data + offset);
                    offset += sizeof(uint16_t);
                    
                    if (offset + len1 > pkt.payload.size())
                    {
                        OutputDebugStringA("[맵 입장] status 길이 초과\n");
                        return;
                    }
                    
                    std::string status(data + offset, len1);

                    std::stringstream ss;
                    ss << "[맵 입장 응답] status: " << status << "\n";
                    OutputDebugStringA(ss.str().c_str());
                }
                catch (...)
                {
                    OutputDebugStringA("[맵 입장] 예외 발생\n");
                }
            }
        );
    }

    // 플레이어 이동 응답 핸들러
    inline void RegisterPlayerMoveHandler()
    {
        NetworkManager::getInstance()->RegisterHandler(PKT_PLAYER_MOVE,
            [](const ParsedPacket& pkt)
            {
                try
                {
                    LOG("[이동 패킷 수신] payload size: " << pkt.payload.size() << "\n");
                    
                    const char* data = pkt.payload.data();
                    size_t offset = 0;
                    size_t payloadSize = pkt.payload.size();

                    if (payloadSize < sizeof(uint16_t))
                    {
                        LOG("[이동 패킷] 페이로드 크기 부족\n");
                        return;
                    }

                    // 1. playerID (char_id)
                    uint16_t len1 = *(uint16_t*)(data + offset);
                    offset += sizeof(uint16_t);
                    
                    if (offset + len1 > payloadSize)
                    {
                        LOG("[이동 패킷] playerID 길이 초과\n");
                        return;
                    }
                    
                    std::string charId(data + offset, len1);
                    offset += len1;
                    LOG("[이동 패킷] playerID: " << charId << "\n");

                    // 내 캐릭터 ID와 같으면 무시 (자기 자신)
                    if (charId == NetworkConfig::GetCharacterId())
                    {
                        LOG("[이동 패킷] 내 캐릭터 무시\n");
                        return;
                    }

                    // 2. xPos
                    if (offset + sizeof(uint16_t) > payloadSize)
                    {
                        LOG("[이동 패킷] xPos 헤더 부족\n");
                        return;
                    }
                    
                    uint16_t len2 = *(uint16_t*)(data + offset);
                    offset += sizeof(uint16_t);
                    
                    if (offset + len2 > payloadSize)
                    {
                        LOG("[이동 패킷] xPos 길이 초과\n");
                        return;
                    }
                    
                    std::string xStr(data + offset, len2);
                    offset += len2;
                    LOG("[이동 패킷] xPos: " << xStr << "\n");

                    // 3. yPos
                    if (offset + sizeof(uint16_t) > payloadSize)
                    {
                        LOG("[이동 패킷] yPos 헤더 부족\n");
                        return;
                    }
                    
                    uint16_t len3 = *(uint16_t*)(data + offset);
                    offset += sizeof(uint16_t);
                    
                    if (offset + len3 > payloadSize)
                    {
                        LOG("[이동 패킷] yPos 길이 초과\n");
                        return;
                    }
                    
                    std::string yStr(data + offset, len3);
                    offset += len3;
                    LOG("[이동 패킷] yPos: " << yStr << "\n");

                    // 4. speed
                    if (offset + sizeof(uint16_t) > payloadSize)
                    {
                        LOG("[이동 패킷] speed 헤더 부족\n");
                        return;
                    }
                    
                    uint16_t len4 = *(uint16_t*)(data + offset);
                    offset += sizeof(uint16_t);
                    
                    if (offset + len4 > payloadSize)
                    {
                        LOG("[이동 패킷] speed 길이 초과\n");
                        return;
                    }
                    
                    std::string speedStr(data + offset, len4);
                    LOG("[이동 패킷] speed: " << speedStr << "\n");

                    float x = (float)atof(xStr.c_str());
                    float y = (float)atof(yStr.c_str());
                    float speed = (float)atof(speedStr.c_str());

                    // LOG("[이동 성공!] Char " << charId << " -> (" << x << ", " << y << ") speed: " << speed << "\n");

                    // 다른 플레이어 위치 업데이트
                    auto otherPlayerMgr = OtherPlayerManager::getInstance();
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
                        LOG("[이동 패킷] OtherPlayerManager가 nullptr!\n");
                    }
                }
                catch (const std::exception& e)
                {
                    std::string msg = "[이동 패킷] 예외 발생: ";
                    msg += e.what();
                    msg += "\n";
                    LOG(msg);
                }
                catch (...)
                {
                    LOG("[이동 패킷] 알 수 없는 예외 발생\n");
                }
            }
        );
    }

    // 모든 핸들러 등록
    inline void RegisterAllHandlers()
    {
        RegisterChannelAuthHandler();
        RegisterEnterMapHandler();
        RegisterPlayerMoveHandler();
        OutputDebugStringA("패킷 핸들러 등록 완료\n");
    }
}
