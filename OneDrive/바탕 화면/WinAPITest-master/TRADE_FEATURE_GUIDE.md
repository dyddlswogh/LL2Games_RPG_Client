# 교환(Trade) 기능 구현 가이드

> 이 문서는 WinAPITest-master 프로젝트의 기존 코드 패턴을 기반으로,  
> **플레이어 간 아이템 교환 기능**을 어떻게 추가하면 되는지 단계별로 안내합니다.

---

## 1. 프로젝트 아키텍처 요약

```
[서버 수신]
  MySocket::Receive()
    └─ NetworkManager::ProcessReceivedData()
         └─ PacketParser::Parse()
              └─ NetworkManager::HandlePacket()
                   └─ 등록된 핸들러 호출 (map<uint16_t, PacketHandler>)

[핸들러 등록]
  PacketManager::RegisterAllHandlers()
    └─ networkManager->RegisterHandler(PKT_XXX, [](const ParsedPacket& pkt){ ... })

[UI 관리]
  UIManager
    └─ vector<UI*> mUIs
         └─ InventoryUI, QuickSlotUI, HealthBarUI ...
```

**핵심 패턴 3가지:**
1. `Packet.h` → 새 OP 코드 추가
2. `XxxPacketHandler` → 수신 파싱 + 송신 함수
3. `XxxUI : public UI` → 창 렌더링 + 입력 처리

---

## 2. 교환 흐름 설계

```
[클라이언트 A]                         [서버]                    [클라이언트 B]
  교환 신청 버튼 클릭
  → SendTradeRequest(targetPlayerId)  →  처리
                                         → PKT_TRADE_REQUEST  →  팝업 표시
                                                                   수락 클릭
                                      ←  SendTradeAccept()   ←
  PKT_TRADE_ACCEPTED 수신
  TradeUI 열림                                                   TradeUI 열림

  아이템 슬롯에 드래그
  → SendTradeAddItem(itemInfo)        →  처리
                                         → PKT_TRADE_ITEM_UPDATE → 상대방 UI 갱신

  교환 확정 버튼 클릭
  → SendTradeConfirm()                →  처리
                                         → PKT_TRADE_COMPLETE  →  양쪽 인벤토리 갱신
```

---

## 3. STEP 1 — Packet.h에 OP 코드 추가

**파일:** `WinAPITest_Source/Packet.h`

기존 패턴:
```cpp
// 0x0080 ~ 0x009F : 인벤토리
PKT_INVENTORY_META_INFO = 0x0080,
PKT_INVENTORY_ITEM_INFO = 0x0081,
```

추가할 내용 (0x00A0 ~ 0x00BF 대역 사용 권장):
```cpp
// 0x00A0 ~ 0x00BF : 교환(Trade)
PKT_TRADE_REQUEST       = 0x00A0,  // C→S: 교환 신청 / S→C: 교환 신청 수신
PKT_TRADE_ACCEPT        = 0x00A1,  // C→S: 교환 수락 / S→C: 수락 통보
PKT_TRADE_DECLINE       = 0x00A2,  // C→S: 교환 거절 / S→C: 거절 통보
PKT_TRADE_ADD_ITEM      = 0x00A3,  // C→S: 아이템 등록
PKT_TRADE_REMOVE_ITEM   = 0x00A4,  // C→S: 아이템 취소
PKT_TRADE_ITEM_UPDATE   = 0x00A5,  // S→C: 상대방 아이템 목록 갱신
PKT_TRADE_CONFIRM       = 0x00A6,  // C→S: 교환 확정(준비 완료)
PKT_TRADE_COMPLETE      = 0x00A7,  // S→C: 교환 완료 (양쪽 인벤 갱신)
PKT_TRADE_CANCEL        = 0x00A8,  // C↔S: 교환 취소
```

---

## 4. STEP 2 — Trade 데이터 구조체 정의

**새 파일 생성:** `WinAPITest_Source/Trade_Info.h`

기존 `Inventory_Info.h` 패턴을 그대로 따릅니다:

```cpp
#pragma once
#include "CommonInclude.h"
#include "Inventory_Info.h"

// 교환 슬롯 하나 (아이템 정보 + 슬롯 위치)
struct TradeSlotInfo
{
    int slotPos   = 0;
    int itemId    = 0;
    int itemCount = 0;
};

// 교환 세션 전체 상태
struct TradeSessionInfo
{
    int myPlayerId     = 0;
    int targetPlayerId = 0;

    std::vector<TradeSlotInfo> myItems;      // 내가 올린 아이템
    std::vector<TradeSlotInfo> targetItems;  // 상대가 올린 아이템

    bool myConfirmed     = false;
    bool targetConfirmed = false;
};

// 교환 신청 수신 시 팝업에 쓸 정보
struct TradeRequestInfo
{
    int    requesterId = 0;
    std::string requesterName;
};

// 교환 완료 결과
struct TradeCompleteResult
{
    int result   = 0;  // 0 = 실패, 1 = 성공
    int errcode  = 0;
    // 완료 후 인벤토리는 PKT_INVENTORY_ITEM_INFO로 별도 갱신됨
};
```

---

## 5. STEP 3 — TradeManager (싱글톤) 생성

**새 파일:** `WinAPITest_Source/TradeManager.h`

기존 `InventoryManager` 패턴 그대로:

```cpp
#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"
#include "Trade_Info.h"

class TradeManager : public stb::SingletonBase<TradeManager>
{
public:
    // 교환 세션 시작/종료
    void StartSession(int myId, int targetId);
    void EndSession();
    bool IsInTrade() const { return m_inTrade; }

    // 아이템 등록/취소
    bool AddMyItem(const TradeSlotInfo& item);
    bool RemoveMyItem(int slotPos);

    // 서버에서 받은 상대방 아이템 갱신
    void UpdateTargetItems(const std::vector<TradeSlotInfo>& items);

    // 확정 상태
    void SetMyConfirmed(bool confirmed);
    void SetTargetConfirmed(bool confirmed);

    const TradeSessionInfo& GetSession() const { return m_session; }
    TradeSessionInfo&       GetSession()       { return m_session; }

private:
    bool             m_inTrade = false;
    TradeSessionInfo m_session;
};
```

---

## 6. STEP 4 — TradePacketHandler 생성

**새 파일:** `WinAPITest_Source/TradePacketHandler.h`

기존 `ItemPacketHandler.h` 패턴 그대로:

```cpp
#pragma once
#include "IPacketHandler.h"
#include "Packet.h"
#include "CommonInclude.h"
#include "Trade_Info.h"

class TradePacketHandler : public IPacketHandler
{
public:
    // ── S→C 수신 핸들러 ──────────────────────────────
    static void HandleTradeRequest   (const ParsedPacket& pkt); // 교환 신청 받음
    static void HandleTradeAccepted  (const ParsedPacket& pkt); // 상대가 수락
    static void HandleTradeDeclined  (const ParsedPacket& pkt); // 상대가 거절
    static void HandleTradeItemUpdate(const ParsedPacket& pkt); // 상대 아이템 목록 갱신
    static void HandleTradeComplete  (const ParsedPacket& pkt); // 교환 완료
    static void HandleTradeCancel    (const ParsedPacket& pkt); // 교환 취소

    // ── C→S 송신 함수 ────────────────────────────────
    static void SendTradeRequest (int targetPlayerId);
    static void SendTradeAccept  ();
    static void SendTradeDecline ();
    static void SendTradeAddItem (const TradeSlotInfo& item);
    static void SendTradeRemoveItem(int slotPos);
    static void SendTradeConfirm ();
    static void SendTradeCancel  ();
};
```

**새 파일:** `WinAPITest_Source/TradePacketHandler.cpp`

`ItemPacketHandler.cpp`의 `SendUseItem` / `HandleUseItemResult` 패턴을 그대로 따릅니다:

```cpp
#include "TradePacketHandler.h"
#include "PacketParser.h"
#include "stbNetworkManager.h"
#include "TradeManager.h"
#include "UIManager.h"

#define M_NETWORK  stb::SingletonBase<stb::NetworkManager>::getInstance()
#define M_TRADEMGR stb::SingletonBase<TradeManager>::getInstance()

// ── 송신 ─────────────────────────────────────────────────────────────

void TradePacketHandler::SendTradeRequest(int targetPlayerId)
{
    // PacketParser::MakeBody는 문자열 벡터를 받아 length-prefix 인코딩
    std::vector<std::string> datas = { std::to_string(targetPlayerId) };
    M_NETWORK->SendPacket(PKT_TRADE_REQUEST, datas);
}

void TradePacketHandler::SendTradeAccept()
{
    M_NETWORK->SendPacket(PKT_TRADE_ACCEPT, {});
}

void TradePacketHandler::SendTradeDecline()
{
    M_NETWORK->SendPacket(PKT_TRADE_DECLINE, {});
}

void TradePacketHandler::SendTradeAddItem(const TradeSlotInfo& item)
{
    std::vector<std::string> datas = {
        std::to_string(item.slotPos),
        std::to_string(item.itemId),
        std::to_string(item.itemCount)
    };
    M_NETWORK->SendPacket(PKT_TRADE_ADD_ITEM, datas);
}

void TradePacketHandler::SendTradeConfirm()
{
    M_NETWORK->SendPacket(PKT_TRADE_CONFIRM, {});
}

void TradePacketHandler::SendTradeCancel()
{
    M_NETWORK->SendPacket(PKT_TRADE_CANCEL, {});
}

// ── 수신 ─────────────────────────────────────────────────────────────

// S→C: 교환 신청이 들어왔을 때
// payload: [requesterId(int)] [requesterName(string)]
void TradePacketHandler::HandleTradeRequest(const ParsedPacket& pkt)
{
    size_t offset = 0;
    const char* data = pkt.payload.c_str();
    size_t payloadSize = pkt.payload.size();
    std::string errMsg;

    TradeRequestInfo info;
    if (!PacketParser::ParseNextIntField(data, payloadSize, offset, info.requesterId, errMsg))
        return;
    if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, info.requesterName, errMsg))
        return;

    // TODO: UIManager에 교환 신청 팝업 표시
    // UIManager::getInstance()->ShowTradeRequestPopup(info);
}

// S→C: 상대방이 수락 → TradeUI 열기
void TradePacketHandler::HandleTradeAccepted(const ParsedPacket& pkt)
{
    size_t offset = 0;
    const char* data = pkt.payload.c_str();
    size_t payloadSize = pkt.payload.size();
    std::string errMsg;

    int targetPlayerId = 0;
    if (!PacketParser::ParseNextIntField(data, payloadSize, offset, targetPlayerId, errMsg))
        return;

    // TradeManager에 세션 시작
    // int myId = PlayerManager::getInstance()->GetLocalPlayer()->GetPlayerIdentity()->playerId;
    // M_TRADEMGR->StartSession(myId, targetPlayerId);

    // TODO: UIManager에 TradeUI 열기
    // UIManager::getInstance()->OpenTradeUI();
}

// S→C: 상대방 아이템 목록 갱신
// payload: [itemCount(int)] { [slotPos] [itemId] [itemCount] } * N
void TradePacketHandler::HandleTradeItemUpdate(const ParsedPacket& pkt)
{
    size_t offset = 0;
    const char* data = pkt.payload.c_str();
    size_t payloadSize = pkt.payload.size();
    std::string errMsg;

    int itemCount = 0;
    if (!PacketParser::ParseNextIntField(data, payloadSize, offset, itemCount, errMsg))
        return;

    std::vector<TradeSlotInfo> items;
    for (int i = 0; i < itemCount; i++)
    {
        TradeSlotInfo slot;
        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, slot.slotPos,   errMsg)) return;
        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, slot.itemId,    errMsg)) return;
        if (!PacketParser::ParseNextIntField(data, payloadSize, offset, slot.itemCount, errMsg)) return;
        items.push_back(slot);
    }

    M_TRADEMGR->UpdateTargetItems(items);
    // TODO: TradeUI 갱신 신호
}

// S→C: 교환 완료
// payload: [result(int)] [errcode(int)]
void TradePacketHandler::HandleTradeComplete(const ParsedPacket& pkt)
{
    size_t offset = 0;
    const char* data = pkt.payload.c_str();
    size_t payloadSize = pkt.payload.size();
    std::string errMsg;

    TradeCompleteResult result;
    if (!PacketParser::ParseNextIntField(data, payloadSize, offset, result.result,  errMsg)) return;
    if (!PacketParser::ParseNextIntField(data, payloadSize, offset, result.errcode, errMsg)) return;

    M_TRADEMGR->EndSession();
    // TODO: TradeUI 닫기, 인벤토리 갱신 (서버가 PKT_INVENTORY_ITEM_INFO 별도 전송)
}

void TradePacketHandler::HandleTradeCancel(const ParsedPacket& pkt)
{
    M_TRADEMGR->EndSession();
    // TODO: TradeUI 닫기, 취소 메시지 표시
}
```

---

## 7. STEP 5 — PacketManager에 핸들러 등록

**파일:** `WinAPITest_Source/PacketManager.cpp`

기존 패턴 그대로 추가:

```cpp
// 상단 include 추가
#include "TradePacketHandler.h"

// RegisterAllHandlers() 안에 추가
networkManager->RegisterHandler(PKT_TRADE_REQUEST,
    [](const ParsedPacket& pkt) {
        TradePacketHandler::HandleTradeRequest(pkt);
    });

networkManager->RegisterHandler(PKT_TRADE_ACCEPT,
    [](const ParsedPacket& pkt) {
        TradePacketHandler::HandleTradeAccepted(pkt);
    });

networkManager->RegisterHandler(PKT_TRADE_ITEM_UPDATE,
    [](const ParsedPacket& pkt) {
        TradePacketHandler::HandleTradeItemUpdate(pkt);
    });

networkManager->RegisterHandler(PKT_TRADE_COMPLETE,
    [](const ParsedPacket& pkt) {
        TradePacketHandler::HandleTradeComplete(pkt);
    });

networkManager->RegisterHandler(PKT_TRADE_CANCEL,
    [](const ParsedPacket& pkt) {
        TradePacketHandler::HandleTradeCancel(pkt);
    });
```

---

## 8. STEP 6 — TradeUI 생성

**새 파일:** `WinAPITest_Source/TradeUI.h`

`InventoryUI.h` 패턴 그대로:

```cpp
#pragma once
#include "UI.h"
#include "Trade_Info.h"

class stbD2DRenderer;

class TradeUI : public UI
{
public:
    void Init()   override;
    void Update() override;
    void Render(HDC hdc)            override;
    void Render(stbD2DRenderer& renderer) override;

private:
    // 내 아이템 슬롯 영역 (왼쪽 패널)
    void RenderMySlots   (stbD2DRenderer& renderer);
    // 상대 아이템 슬롯 영역 (오른쪽 패널)
    void RenderTargetSlots(stbD2DRenderer& renderer);
    // 확정/취소 버튼
    void RenderButtons   (stbD2DRenderer& renderer);

    void HandleLMouseClick(int mouseX, int mouseY);
    int  GetClickedMySlotIndex(int mouseX, int mouseY);

private:
    int m_posX = 200;
    int m_posY = 150;

    // 슬롯 레이아웃 (InventoryUI와 동일한 방식)
    static constexpr int SLOT_COLS  = 4;
    static constexpr int SLOT_ROWS  = 4;
    static constexpr int SLOT_W     = 32;
    static constexpr int SLOT_H     = 32;
    static constexpr int SLOT_GAP_X = 2;
    static constexpr int SLOT_GAP_Y = 2;
};
```

**TradeUI.cpp 핵심 구현 포인트:**

```cpp
// Update() — InventoryUI::Update()와 동일한 구조
void TradeUI::Update()
{
    if (!mActive) return;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(M_APP->GetHWND(), &pt);

    if (M_INPUT->GetKeyDown(stb::eKeyCode::LButton))
        HandleLMouseClick(pt.x, pt.y);
}

// 내 슬롯 클릭 → 아이템 교환창에 등록
void TradeUI::HandleLMouseClick(int mouseX, int mouseY)
{
    int slotIdx = GetClickedMySlotIndex(mouseX, mouseY);
    if (slotIdx == -1) return;

    // 인벤토리에서 해당 슬롯 아이템 가져오기
    // InventoryItemInfo* item = M_INVENTORYMANAGER->FindSlot(inventoryType, slotIdx);
    // if (item == nullptr) return;

    // TradeSlotInfo 만들어서 서버에 전송
    // TradeSlotInfo tradeSlot = { slotIdx, item->itemId, item->itemCount };
    // TradePacketHandler::SendTradeAddItem(tradeSlot);
}
```

---

## 9. STEP 7 — UIManager에 TradeUI 등록

**파일:** `WinAPITest_Source/UIManager.h` — 멤버 추가:

```cpp
#include "TradeUI.h"

class UIManager : public stb::SingletonBase<UIManager>
{
public:
    // ... 기존 코드 ...
    void OpenTradeUI();
    void CloseTradeUI();
    void ShowTradeRequestPopup(const TradeRequestInfo& info); // 신청 팝업

private:
    // ... 기존 멤버 ...
    TradeUI* m_tradeUI = nullptr;
};
```

**파일:** `WinAPITest_Source/UIManager.cpp` — Init()에 추가:

```cpp
UIManager::UIManager()
{
    m_inventoryUI = new InventoryUI();
    m_quickslotUI = new QuickSlotUI();
    m_healthBarUI = new HealthBarUI();
    m_tradeUI     = new TradeUI();      // 추가
}

void UIManager::Init()
{
    m_inventoryUI->Init();
    m_quickslotUI->Init();
    m_healthBarUI->Init();
    m_tradeUI->Init();                  // 추가

    mUIs.push_back(m_inventoryUI);
    mUIs.push_back(m_quickslotUI);
    mUIs.push_back(m_healthBarUI);
    mUIs.push_back(m_tradeUI);          // 추가
}

void UIManager::OpenTradeUI()
{
    m_tradeUI->SetActivce(true);
}
```

---

## 10. 전체 파일 변경/생성 목록

| 작업 | 파일 | 내용 |
|------|------|------|
| **수정** | `Packet.h` | `PKT_TRADE_*` OP 코드 8개 추가 |
| **신규** | `Trade_Info.h` | `TradeSlotInfo`, `TradeSessionInfo`, `TradeRequestInfo` 구조체 |
| **신규** | `TradeManager.h/.cpp` | 교환 세션 상태 관리 싱글톤 |
| **신규** | `TradePacketHandler.h/.cpp` | 송수신 함수 전부 |
| **수정** | `PacketManager.cpp` | `RegisterAllHandlers()`에 Trade 핸들러 5개 등록 |
| **신규** | `TradeUI.h/.cpp` | 교환 창 UI |
| **수정** | `UIManager.h/.cpp` | TradeUI 멤버 추가, Init/Open/Close 함수 |

---

## 11. PacketParser 사용법 요약

기존 코드에서 쓰는 파싱 함수 3가지:

```cpp
// int 파싱 (내부적으로 length-prefix string → stoi 변환)
PacketParser::ParseNextIntField(data, payloadSize, offset, outInt, errMsg);

// float 파싱
PacketParser::ParseNextFloatField(data, payloadSize, offset, outFloat, errMsg);

// 문자열 파싱 (length-prefix)
PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, outStr, errMsg);

// 패킷 송신 (문자열 벡터 → body → 패킷)
M_NETWORK->SendPacket(PKT_TRADE_REQUEST, { std::to_string(targetId) });
```

모든 파싱은 `try { ... } catch(std::runtime_error& e) { ... }` 로 감싸는 것이 기존 패턴입니다 (`InventoryPacketHandler.cpp` 참고).

---

## 12. 구현 순서 권장

1. `Packet.h` — OP 코드 추가 (컴파일 확인)
2. `Trade_Info.h` — 구조체 정의
3. `TradeManager.h/.cpp` — 싱글톤 뼈대만 (빈 함수 OK)
4. `TradePacketHandler.h/.cpp` — Send 함수부터 구현 → 서버 연동 테스트
5. `PacketManager.cpp` — 핸들러 등록
6. Handle 함수 구현 (수신 파싱)
7. `TradeUI.h/.cpp` — UI 뼈대 (mActive=false로 시작)
8. `UIManager` — TradeUI 등록
9. 교환 신청 버튼 → `SendTradeRequest()` 연결
10. 전체 흐름 통합 테스트
