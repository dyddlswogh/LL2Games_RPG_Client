#include "ChatPacketHandler.h"

#include "PacketParser.h"
#include "stbChatNetworkManager.h"
#include "UIManager.h"

#define M_CHAT_NETWORK stb::SingletonBase<stb::ChatNetworkManager>::getInstance()
//#define M_TRADEMGR stb::SingletonBase<TradeManager>::getInstance()

// ── 송신 ─────────────────────────────────────────────────────────────


//채팅 초기화
void ChatPacketHandler::SendChatInit(const std::string& charId)
{
	std::vector<std::string> datas = { charId };
	M_CHAT_NETWORK->SendPacket(PKT_CHAT_INIT, datas);

	// TODO: UIManager에 TradeUI 열기
}

void ChatPacketHandler::SendChat(const std::string& msg)
{
	std::vector<std::string> datas = { msg };
	M_CHAT_NETWORK->SendPacket(PKT_CHAT, datas);

	// TODO: UIManager에 TradeUI 열기
}

// ── 수신 ─────────────────────────────────────────────────────────────

// 
// S→C: 채팅 초기화 응답
//payload: [requesterId(string)][requesterName(string)]
void ChatPacketHandler::HandleChatInit(const ParsedPacket& pkt)
{
	size_t offset = 0;
	const char* data = pkt.payload.c_str();
	size_t payloadSize = pkt.payload.size();
	std::string errMsg;
	std::string status, nick;

	if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, status, errMsg))
		return;

	if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, nick, errMsg))
		return;

	//TODO
	//status에 따라 성공/실패처리
}

// S→C: 채팅 초기화 응답
void ChatPacketHandler::HandleChat(const ParsedPacket& pkt)
{
	size_t offset = 0;
	const char* data = pkt.payload.c_str();
	size_t payloadSize = pkt.payload.size();
	std::string errMsg;
	std::string status, nick, msg;

	if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, status, errMsg))
		return;

	if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, nick, errMsg))
		return;

	if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, msg, errMsg))
		return;

	//TODO
	//status에 따라 성공/실패처리
	//채팅 아이디, 메시지
}
