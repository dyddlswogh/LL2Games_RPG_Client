#pragma once
#include "IPacketHandler.h"
#include "Packet.h"
#include "CommonInclude.h"

class MovePacketHandler : public IPacketHandler
{
public:
	static void Execute(const ParsedPacket& pkt);

private:
};

