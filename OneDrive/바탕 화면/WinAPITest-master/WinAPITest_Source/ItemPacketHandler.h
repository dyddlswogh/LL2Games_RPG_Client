#pragma once
#include "IPacketHandler.h"
#include "Packet.h"
#include "CommonInclude.h"


class ItemPacketHandler : public IPacketHandler
{
public:
	static void Execute(const ParsedPacket& pkt);
	static void SendUseItem();
	
private:
};


