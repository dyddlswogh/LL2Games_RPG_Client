#include "PacketManager.h"
#include "Packet.h"
#include "stbNetworkManager.h"
#include "MovePacketHandler.h"
#include "ChannelInitPacketHandler.h"

bool PacketManager::RegisterAllHandlers()
{
	auto networkManager = stb::NetworkManager::getInstance();

	// 플레이어 접속 핸들러 등록
	networkManager->RegisterHandler(PKT_CHANNEL_AUTH,
		[](const ParsedPacket& pkt)
		{
			ChannelInitPacketHandler::Execute(pkt);
		});

	// 플레이어 움직임 핸들러 등록
	networkManager->RegisterHandler(PKT_PLAYER_MOVE,
		[](const ParsedPacket& pkt)
		{
			MovePacketHandler::Execute(pkt);
		});




	return true;
}