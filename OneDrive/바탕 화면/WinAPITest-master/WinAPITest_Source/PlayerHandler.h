#pragma once
#include "CommonInclude.h"
#include "IPacketHandler.h"




class PlayerHandler : public IPacketHandler
{
public:

	void Execute(PacketContext* ctx) override;

	void MovePacket(PacketContext* ctx);
	void AttackPacket(PacketContext* ctx);
	void OnDamagedPacket(PacketContext* ctx);
	void UseItemPacket(PacketContext* ctx);

private:
};

