#include "TradePacketHandler.h"
#include "PacketParser.h"
#include "stbNetworkManager.h"
#include "TradeManager.h"
#include "UIManager.h"

#define M_NETWORK stb::SingletonBase<stb::NetworkManager>::getInstance()
#define M_TRADEMGR stb::SingletonBase<TradeManager>::getInstance()

// ── 송신 ─────────────────────────────────────────────────────────────

//교환 신청
void TradePacketHandler::SendTradeRequest(std::string targetPlayerId)
{
	//PacketParser::MakeBody는 문자열 벡터를 받아 length-prefix인코딩
	std::vector<std::string> datas = { targetPlayerId };
	M_NETWORK->SendPacket(PKT_TRADE_REQUEST, datas);

	// TODO: UIManager에 TradeUI 열기
	// UIManager::getInstance()->OpenTradeUI();
}

//교환 수락
void TradePacketHandler::SendTradeAccept()
{
	M_NETWORK->SendPacket(PKT_TRADE_ACCEPT, {});
}

//교환 거절
void TradePacketHandler::SendTradeDecline()
{
	//거절은 보낼 필요 없을듯 -> 메이플에 거절했다고 상대에게 알림 안감.
	//M_NETWORK->SendPacket(PKT_TRADE_DECLINE, {});
}

//아이템 추가
void TradePacketHandler::SendTradeAddItem(const TradeSlotInfo& item)
{
	std::vector<std::string> datas = {
		std::to_string(item.slotPos),
		item.itemId,
		std::to_string(item.itemCount)
	};

	M_NETWORK->SendPacket(PKT_TRADE_ADD_ITEM, datas);
}

//교환 취소
void TradePacketHandler::SendTradeCancel()
{
	M_NETWORK->SendPacket(PKT_TRADE_CANCEL, {});
}

// ── 수신 ─────────────────────────────────────────────────────────────

// S→C: 교환 신청이 들어왔을 때
//payload: [requesterId(string)][requesterName(string)]
void TradePacketHandler::HandleTradeRequest(const ParsedPacket& pkt)
{
	size_t offset = 0;
	const char* data = pkt.payload.c_str();
	size_t payloadSize = pkt.payload.size();
	std::string errMsg;

	TradeRequestInfo info;
	if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, info.requesterId, errMsg))
		return;
		
	if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, info.requesterName, errMsg))
		return;

	//TODO: UIManager에 교환 신청 팝업 표시
	UIManager::getInstance()->ShowTradeRequestPopUp(info);

}

//void TradePacketHandler::HandleTradeRequest(const ParsedPacket& pkt)