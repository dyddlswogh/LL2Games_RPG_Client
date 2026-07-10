# LL2Games_RPG Client

C++/WinAPI 기반 2D MMORPG 클라이언트입니다.  
기존 WinAPI 클라이언트 구조와 게임 리소스를 프로젝트에 통합하고, Direct2D 렌더링 흐름과 서버 패킷 연동을 확장하여 실제 MMORPG 플레이 화면을 구현했습니다.

서버와 TCP 패킷으로 연동하여 Channel 인증, 맵 입장, 플레이어 이동, 다른 플레이어 동기화, 전투, 몬스터, 아이템, 인벤토리, 퀵슬롯, 채팅, 교환 기능을 확인할 수 있습니다.

## Demo

> GIF와 플레이 영상은 포트폴리오 제출 전 추가 예정입니다.

| 구분 | 내용 |
| --- | --- |
| Play Video | 추가 예정 |
| Movement / Multiplayer Sync | 추가 예정 |
| Combat / Monster | 추가 예정 |
| Item / Inventory / QuickSlot | 추가 예정 |
| Chat / Trade UI | 추가 예정 |

## Highlights

- C++/WinAPI 기반 2D 게임 클라이언트 구조
- GDI+ 중심 렌더링 흐름을 Direct2D 기반으로 전환
- Application / Scene / GameObject / Component / ResourceManager 기반 구조 활용
- TCP socket 기반 서버 패킷 송수신
- PacketManager / opcode handler 기반 패킷 처리
- Channel 인증, 맵 입장, 이동, 전투, 아이템, 인벤토리, 퀵슬롯 연동
- 같은 맵의 다른 플레이어 위치, 방향, 상태, 애니메이션 동기화
- JSON 기반 아이템 및 애니메이션 데이터 로딩
- MFC 기반 로그인, 채팅, 교환 UI 연동

## Tech Stack

| Area | Tech |
| --- | --- |
| Language | C++ |
| Client | WinAPI, MFC |
| Rendering | Direct2D, GDI+ |
| Network | TCP Socket |
| Data | JSON |
| Tool | Visual Studio |

## Architecture

```text
Application
 ├─ Scene
 ├─ GameObject
 │   └─ Component
 ├─ ResourceManager
 ├─ PacketManager
 │   └─ PacketHandler
 ├─ Player / OtherPlayer / Monster
 ├─ Inventory / QuickSlot
 └─ UI
```

## Core Features

### 1. Client Runtime

- Application, Scene, GameObject, Component 기반 클라이언트 구조 활용
- ResourceManager 기반 리소스 로딩
- Direct2D 기반 렌더링 흐름 적용
- 입력, 업데이트, 렌더링 루프 기반 게임 화면 구성

### 2. Server Packet Integration

- TCP socket 기반 서버 연결
- PacketManager와 opcode별 handler 구성
- Channel 인증, 맵 입장, Player/Map/Inventory 데이터 수신
- 서버 패킷을 클라이언트 객체와 UI 상태에 반영

### 3. Player & Multiplayer Sync

- 내 플레이어 이동, 방향, 상태 처리
- 같은 맵의 다른 플레이어 입장 및 snapshot 처리
- 다른 플레이어 이동, 방향, Idle/Walk/Attack 애니메이션 동기화
- 서버 broadcast 패킷 기반 화면 상태 갱신

### 4. Combat & Monster

- 전투 요청 패킷 전송
- 몬스터 생성, 피격, 리스폰 처리
- 데미지, 경험치, 상태 변경 패킷 반영
- 공격 애니메이션과 서버 전투 결과 연결

### 5. Item / Inventory / QuickSlot

- 서버 아이템 데이터와 클라이언트 UI 연동
- 아이템 드롭, 줍기, 인벤토리 반영
- 타입별 인벤토리 탭, 슬롯, 확장, 드래그 이동 처리
- 퀵슬롯 등록 및 아이템 사용 처리

### 6. UI

- HP/MP, 경험치, 인벤토리, 퀵슬롯 UI
- MFC 기반 로그인 창, 채팅 창, 교환 창 연동
- 서버 패킷 결과를 UI 상태에 반영

## Client-Server Flow

```text
Connect Server
 → Channel Auth
 → Receive Player / Map / Inventory Data
 → Enter Map
 → Receive Other Player Snapshot
 → Move / Attack / Item Packet
 → Update Client Object & UI State
```

## Contribution

2인 협업 프로젝트입니다.  
전체 클라이언트 기능과 개인 구현 범위를 구분하여 정리했습니다.

### 개인 기여

- 기존 WinAPI 클라이언트 구조와 게임 리소스를 프로젝트에 통합
- GDI+ 중심 렌더링 경로를 Direct2D 기반으로 전환
- PacketManager와 opcode별 packet handler 구성
- Channel 인증부터 맵 입장까지 이어지는 서버 패킷 연동 구현
- 플레이어 이동, 다른 플레이어 snapshot/이동/공격 동기화 구현
- 몬스터 생성/피격/리스폰, 전투 요청, 경험치 반영 구현
- 아이템 드롭/줍기, 인벤토리, 퀵슬롯, 아이템 사용 UI 구현
- JSON 기반 아이템 및 애니메이션 데이터 로딩 구현

### 협업 기여

- 협업자는 MFC 기반 로그인 창, 채팅 창, 교환 창을 구현
- 해당 UI를 서버 패킷 처리 흐름과 연동하여 프로젝트 전체 기능으로 통합

## Related Links
- Server Repository: https://github.com/LL2Games/LL2Games_RPG_Server
## Notes

- 서버와 함께 실행해야 전체 기능을 확인할 수 있습니다.
- 서버 주소, 포트, 테스트 캐릭터 정보는 실행 환경에 맞게 설정해야 합니다.
- 포트폴리오 제출용 README에서는 Demo 영역에 실제 GIF와 플레이 영상을 추가하는 것을 권장합니다.
