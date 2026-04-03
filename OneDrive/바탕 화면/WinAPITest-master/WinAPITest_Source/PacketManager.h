#pragma once
#include "CommonInclude.h"
#include "Packet.h"
#include "stbNetworkManager.h"
#include "MovePacketHandler.h"

class PacketManager 
{
public:
	static PacketManager* getInstance();

	bool RegisterAllHandlers();


private:
	static PacketManager* m_instance;

};

