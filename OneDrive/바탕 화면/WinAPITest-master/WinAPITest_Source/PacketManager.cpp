#include "PacketManager.h"
#include "Packet.h"
#include "stbNetworkManager.h"
#include "MovePacketHandler.h"
#include "ChannelInitPacketHandler.h"
#include "InventoryPacketHandler.h"
#include "PlayerDataPacketHandler.h"
#include "ItemPacketHandler.h"
#include "MonsterPacketHandler.h"
#include "TradePacketHandler.h"

bool PacketManager::RegisterAllHandlers()
{
	auto networkManager = stb::NetworkManager::getInstance();

	// 플레이어 접속 핸들러 등록
	networkManager->RegisterHandler(PKT_CHANNEL_AUTH,
		[](const ParsedPacket& pkt)
		{
			ChannelInitPacketHandler::Execute(pkt);
		});

	// 플레이어 기본정보 핸들러 등록
	networkManager->RegisterHandler(PKT_PLAYER_INFO,
		[](const ParsedPacket& pkt)
		{
			PlayerDataPacketHandler::HandleLocalPlayerInfo(pkt);
		});

	// 플레이어 스탯 정보 핸들러 등록
	networkManager->RegisterHandler(PKT_PLAYER_STAT,
		[](const ParsedPacket& pkt)
		{
			PlayerDataPacketHandler::HandleLocalPlayerStat(pkt);
		});

	// 플레이어 인벤토리 핸들러 등록
	networkManager->RegisterHandler(PKT_INVENTORY_META_INFO,
		[](const ParsedPacket& pkt)
		{
			InventoryPacketHandler::HandleInventoryMetaInfo(pkt);
		});

	// 플레이어 인벤토리 핸들러 등록
	networkManager->RegisterHandler(PKT_INVENTORY_ITEM_INFO,
		[](const ParsedPacket& pkt)
		{
			InventoryPacketHandler::HandleInventoryItemInfo(pkt);
		});

	// 플레이어 움직임 핸들러 등록
	networkManager->RegisterHandler(PKT_PLAYER_MOVE,
		[](const ParsedPacket& pkt)
		{
			MovePacketHandler::Execute(pkt);
		});

	// 플레이어 아이템 사용 핸들러 등록
	networkManager->RegisterHandler(PKT_PLAYER_USE_ITEM,
		[](const ParsedPacket& pkt)
		{
			ItemPacketHandler::HandleUseItemResult(pkt);
		});

	// 몬스터 스냅샷 핸들러 등록
	networkManager->RegisterHandler(PKT_MONSTER_SNAPSHOT,
		[](const ParsedPacket& pkt)
		{
			MonsterPacketHandler::HandleS2C_SpawnMonster(pkt);
		});

	// 몬스터 업데이트 핸들러 등록
	networkManager->RegisterHandler(PKT_MONSTER_MOVE,
		[](const ParsedPacket& pkt)
		{
			MonsterPacketHandler::HandleS2C_MonsterMove(pkt);
		});

	// 교환 신청 핸들러 등록
	networkManager->RegisterHandler(PKT_TRADE_REQUEST,
		[](const ParsedPacket& pkt)
		{
			TradePacketHandler::HandleTradeRequest(pkt);
		});

	// 교환 시작 핸들러 등록
	networkManager->RegisterHandler(PKT_TRADE_START,
		[](const ParsedPacket& pkt)
		{
			TradePacketHandler::HandleTradeStart(pkt);
		});

	// 교환 취소 핸들러 등록
	networkManager->RegisterHandler(PKT_TRADE_CANCEL,
		[](const ParsedPacket& pkt)
		{
			TradePacketHandler::HandleTradeCancel(pkt);
		});

	// 교환 준비 핸들러 등록
	networkManager->RegisterHandler(PKT_TRADE_READY,
		[](const ParsedPacket& pkt)
		{
			TradePacketHandler::HandleTradeReady(pkt);
		});

	// 교환 완료 핸들러 등록
	networkManager->RegisterHandler(PKT_TRADE_CONFIRM,
		[](const ParsedPacket& pkt)
		{
			TradePacketHandler::HandleTradeComplete(pkt);
		});

	
	return true;
}