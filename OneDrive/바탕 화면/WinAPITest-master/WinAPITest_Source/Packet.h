#pragma once
#include <string>
#include <cstdint>
#include <vector>

#define BUFFER_SIZE 1024
#define WM_SOCKET_RECEIVE (WM_USER + 100)
#define WM_SOCKET_DISCONNECT (WM_USER + 101)

#pragma pack(push,1)
struct PacketHeader
{
    uint16_t length;
    uint16_t type;
};

#pragma pack(pop)

enum PACKET_TYPE : uint16_t {
    PKT_LOGIN = 0x01,
    PKT_REGISTER = 0x02,
    PKT_CHAT_INIT = 0x03,
    PKT_CHAT = 0x04,
    PKT_INIT_WORLD = 0x05,
    PKT_SELECT_CHARACTER = 0x06,
    PKT_SELECT_CHANNEL = 0x07,
    PKT_INIT_CHANNEL = 0x08,
    PKT_CHANNEL_AUTH = 0x09,      // 채널 인증 (캐릭터 ID 전송)
    PKT_ENTER_MAP = 0x0A,          // 맵 입장 (캐릭터 ID, 맵 ID)
    PKT_PLAYER_MOVE = 0x0B,        // 플레이어 이동 (x, y, speed)
    PKT_PLAYER_ATTACK = 0x0C,
    PKT_PLAYER_ONDAMAGED = 0x0D,
    PKT_PLAYER_USE_ITEM = 0x0E,
    PKT_MONSTER_MOVE = 0x0F,
    PKT_MONSTER_ONDAMAGED = 0x10,
    PKT_STAT_VIEW = 0x1000,
    PKT_STAT_UP,
};

struct ParsedPacket
{
    uint16_t type;
    std::string payload;
};

struct PacketContext
{
    uint16_t type;
    int fd = -1;
    char* payload = nullptr;
    int payload_len = 0;
};

typedef struct packet
{
    PacketHeader header{};
    std::vector<uint8_t> payload;
} Packet;