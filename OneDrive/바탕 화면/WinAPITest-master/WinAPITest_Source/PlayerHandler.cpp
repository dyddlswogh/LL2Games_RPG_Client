#include "PlayerHandler.h"


void PlayerHandler::Execute(PacketContext* ctx)
{
	switch (ctx->type)
	{
		case PKT_PLAYER_MOVE:
			break;
		case PKT_PLAYER_ATTACK:
			break;
		case PKT_PLAYER_ONDAMAGED:
			break;
		case PKT_PLAYER_USE_ITEM:
			break;
		case PKT_STAT_VIEW:
			break;
		case PKT_STAT_UP:
			break;
		default:
			break;
	}
}