# WinAPITest Client

WinAPI + Direct2D 기반의 2D 게임 클라이언트입니다. 별도의 서버에 TCP 소켓으로 접속하여 동작합니다.

---

## 서버 접속 정보 설정

**`WinAPITest_Source/stbNetworkConfig.h`** 파일에서 서버 IP와 포트를 수정합니다.

```cpp
struct NetworkConfig
{
    // ========== 여기를 수정하세요! ==========

    static constexpr const char* SERVER_IP   = "100.99.220.45";  // 서버 IP
    static constexpr int         SERVER_PORT = 1234;             // 서버 포트

    static constexpr const char* MAP_ID      = "100000000";      // 맵 ID
    static constexpr bool        AUTO_CONNECT = true;            // 자동 연결 여부
    static constexpr int         RECONNECT_ATTEMPTS = 3;         // 재연결 시도 횟수
};
```

---

## 캐릭터 ID 설정

실행 시 **커맨드라인 인자**로 캐릭터 ID를 전달합니다.

```
WinAPITest_Client.exe 2
```

인자를 생략하면 기본값 `"1"`이 사용됩니다.  
기본값은 `WinAPITest_Source/stbNetworkConfig.cpp`에서 변경할 수 있습니다.

```cpp
char g_CharacterId[32] = "1";  // 기본 캐릭터 ID
```

---

## 프로젝트 구조

| 폴더 | 설명 |
|------|------|
| `WinAPITest_Client/` | 진입점 (`WinMain`), 윈도우 생성 및 메시지 루프 |
| `WinAPITest_Source/` | 엔진 코어, 네트워크, 패킷 처리 소스 |
| `WinAPITest_lib/` | 씬/리소스 로드, 플레이어 스크립트 |
| `Resources/` | 이미지 리소스 |

### 주요 네트워크 파일

| 파일 | 역할 |
|------|------|
| `stbNetworkConfig.h` | **서버 IP / 포트 설정** |
| `stbNetworkManager.h/.cpp` | 소켓 연결 및 패킷 송수신 관리 |
| `stbNetworkInit.h` | 네트워크 초기화 헬퍼 함수 |
| `MySocket.h/.cpp` | 저수준 Winsock 래퍼 |
| `PacketParser.h/.cpp` | 수신 패킷 파싱 |
| `Packet.h` | 패킷 타입 정의 |

---

## 인벤토리

### 데이터 수신 방식

인벤토리 데이터는 서버 접속 후 자동으로 패킷을 통해 수신됩니다. 별도의 키 입력 없이 서버가 아래 패킷을 보내면 클라이언트가 처리합니다.

| 패킷 타입 | 값 | 설명 |
|-----------|-----|------|
| `PKT_INVENTORY_META_INFO` | `0x0080` | 인벤토리 슬롯 구조 초기화 |
| `PKT_INVENTORY_ITEM_INFO` | `0x0081` | 슬롯별 아이템 데이터 수신 |

### 인벤토리 구조

인벤토리는 타입(inventoryType)으로 구분되며, 각 타입마다 슬롯 목록을 가집니다.

```
InventoryManager
 └─ Inventory (inventoryType 별)
      └─ InventorySlot[]
           ├─ slotPos   : 슬롯 번호
           ├─ itemId    : 아이템 ID
           └─ itemCount : 수량
```

### 코드에서 인벤토리 접근

```cpp
// 특정 타입의 인벤토리 가져오기
Inventory* inv = InventoryManager::getInstance()->GetInventory(inventoryType);

// 특정 슬롯 조회
InventorySlot* slot = InventoryManager::getInstance()->FindSlot(inventoryType, slotPos);
```

> 인벤토리 UI(화면 표시 및 키 입력으로 열기)는 현재 미구현 상태입니다.

---

## 빌드 및 실행

1. `WinAPITest.sln`을 Visual Studio로 열기
2. `WinAPITest_Source/stbNetworkConfig.h`에서 서버 IP/포트 수정
3. `x64 / Debug` 구성으로 빌드
4. 실행 (캐릭터 ID는 커맨드라인 인자로 전달)

> 다중 클라이언트 테스트는 `WinAPITest_Source/RunMultipleClients.bat` 참고
