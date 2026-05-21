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
    // 0x0001 ~ 0x001F : 로그인 / 월드
    PKT_LOGIN               = 0x0001,
    PKT_REGISTER            = 0x0002,
    PKT_CHAT_INIT           = 0x0003,
    PKT_CHAT                = 0x0004,
    PKT_INIT_WORLD          = 0x0005,
    PKT_SELECT_CHARACTER    = 0x0006,
    PKT_SELECT_CHANNEL      = 0x0007,
    PKT_INIT_CHANNEL        = 0x0008,
    PKT_CHANNEL_AUTH        = 0x0009,
    PKT_ENTER_MAP           = 0x000A,

    // 0x0020 ~ 0x003F : 플레이어
    PKT_PLAYER_MOVE         = 0x0020,
    PKT_PLAYER_ATTACK       = 0x0021,
    PKT_PLAYER_ONDAMAGED    = 0x0022,
    PKT_PLAYER_USE_ITEM     = 0x0023,
    PKT_PLAYER_INFO         = 0x0024,
    PKT_PLAYER_STAT         = 0x0025,
    PKT_PLAYER_SKILLLIST    = 0x0026,
    PKT_PLAYER_BASIC_ATTACK = 0x0027,

    // 0x0040 ~ 0x005F : 몬스터
    PKT_MONSTER_MOVE        = 0x0040,
    PKT_MONSTER_ONDAMAGED   = 0x0041,
    PKT_MONSTER_SNAPSHOT    = 0x0042,

    // 0x0060 ~ 0x007F : 드롭
    PKT_DROPITEMS           = 0x0060,
    PKT_REMOVEITEMS         = 0x0061,

    // 0x0080 ~ 0x009F : 인벤토리
    PKT_INVENTORY_META_INFO = 0x0080,
    PKT_INVENTORY_ITEM_INFO = 0x0081,
    PKT_INVENTORY_ITEM_MOVE = 0x0082,

    // 0x1000 ~ : 테스트 / UI / 특수
    PKT_STAT_VIEW           = 0x1000,
    PKT_STAT_UP             = 0x1001,
    PKT_QUICKSLOT_LIST      = 0x1002,
    PKT_QUICKSLOT_SET       = 0x1003,
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