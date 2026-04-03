#include "PacketManager.h"

PacketManager* PacketManager::m_instance = nullptr;

PacketManager* PacketManager::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new PacketManager();
	}

	return m_instance;
}

bool PacketManager::RegisterAllHandlers()
{
	auto networkManager = stb::NetworkManager::getInstance();

	// 플레이어 움직임 핸들러 등록
	networkManager->RegisterHandler(PKT_PLAYER_MOVE,
		[](const ParsedPacket& pkt)
		{
			MovePacketHandler::Execute(pkt);
		});


	//networkManager->RegisterHandler(PKT_PLAYER_USE_ITEM,
	//	[](const ParsedPacket& pkt)
	//	{
	//
	//	})

	return true;
}