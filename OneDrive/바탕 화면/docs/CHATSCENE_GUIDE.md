# ChatScene 제작 가이드

> 목표: PlayScene 위에 **덧대어지는 채팅창**을 만들고,
> PlayScene이 쓰는 채널 서버와는 **별도의 채팅 서버 포트**에 접속해서 통신.

선행 학습: [`PLAYSCENE_GUIDE.md`](PLAYSCENE_GUIDE.md)

---

## 0. 한 줄 결론

> **ChatScene 클래스는 PlayScene과 동일한 Scene 인터페이스로 만들되,
> 화면 위에 “덧대는” 동작은 SceneManager의 `DontDestroyOnLoad` 슬롯을 활용하거나
> Application/PlayScene에서 직접 Update·Render를 보조 호출하는 방식으로 처리합니다.
> 네트워크는 PlayScene의 `NetworkManager` 싱글톤을 그대로 쓰지 말고,
> 별도의 `ChatNetworkManager` 싱글톤(또는 ChatScene 내부 소유 `MySocket`)을 사용합니다.**

---

## 1. 두 가지 핵심 문제

### 1-1. “위에 덧대어 그리기” 문제

`SceneManager`는 `mActiveScene`과 `mDontDestroyOnLoad` 두 개만 매 프레임 Update/Render합니다.
PlayScene이 active인 상태에서 ChatScene도 같이 그리려면 다음 중 하나를 선택해야 합니다.

| 방식 | 장점 | 단점 |
|---|---|---|
| **A. DontDestroyOnLoad 슬롯 활용** | SceneManager에 손 안 대고 “Active Scene 위에 항상 그려지는 Scene”을 그대로 활용 | DontDestroyOnLoad가 다른 용도로도 쓰일 가능성. (현재는 비어 있어 보임) |
| **B. SceneManager 확장** (`mOverlayScene` 추가) | 의도가 가장 명확함 | SceneManager 코드 수정 필요 |
| **C. PlayScene 내부에서 ChatScene을 멤버로 보유** | SceneManager 수정 없음, PlayScene과 라이프사이클을 강하게 묶을 수 있음 | Scene 간 책임 분리가 약해짐 |
| **D. UIManager 안의 UI로 구현 (ChatUI)** | 가장 자연스러운 “덧대기”, 기존 UI들과 동일한 패턴 | Scene 클래스를 새로 만들지 않게 됨 (사용자 의도와 다름) |

> 사용자의 요구(“ChatScene을 새로 만든다 + PlayScene처럼 만든다”)에 가장 가까운 것은 **B 또는 A**.
> 이 문서는 **B (SceneManager 확장)** 를 권장 방식으로 설명하고, A는 변형으로 짧게 언급합니다.

### 1-2. “별도 서버 포트” 문제

`stb::NetworkManager`는 **싱글톤 + 내부에 `MySocket` 한 개**입니다.
PlayScene이 채널 서버에 이미 접속해서 그 소켓을 쓰고 있는데,
같은 NetworkManager에서 또 다른 서버에 연결하면 채널 통신이 끊기게 됩니다.

→ 따라서 채팅 서버용으로 **별도의 NetworkManager 인스턴스(=싱글톤 클래스 1개 더)** 를 만들어야 합니다.

또한 현재 `WM_SOCKET_RECEIVE`(=`WM_USER + 100`)는 `NetworkManager` 한 개를 가정하고 분기됩니다.
**채팅용 소켓은 다른 윈도우 메시지 ID**(예: `WM_CHAT_SOCKET_RECEIVE = WM_USER + 102`)
를 사용해 WndProc에서 분리해서 처리해야 합니다.

---

## 2. 권장 아키텍처 (전체 그림)

```
wWinMain
  ├─ CLogin
  ├─ CWorld (g_channel_port + 추가로 g_chat_port 세팅)
  ├─ APP->Initialize
  └─ stb::LoadScene()
        ├─ SCENEMANAGER->CreateScene<PlayScene>(L"PlayScene")
        ├─ SCENEMANAGER->CreateScene<ChatScene>(L"ChatScene")  ← 새로 등록
        ├─ SCENEMANAGER->LoadScene(L"PlayScene")               ← active
        └─ SCENEMANAGER->SetOverlayScene(L"ChatScene")         ← overlay (옵션)

매 프레임:
  Application::Update()
    └─ SceneManager::Update()
         ├─ mActiveScene->Update()      // PlayScene
         ├─ mOverlayScene->Update()     // ChatScene (있을 때만)
         └─ mDontDestroyOnLoad->Update()

  Application::Render()
    └─ SceneManager::Render(renderer)
         ├─ mActiveScene->Render()
         ├─ mOverlayScene->Render()     // 위에 덧그림
         └─ mDontDestroyOnLoad->Render()

윈도우 메시지:
  case WM_SOCKET_RECEIVE:        → NetworkManager      (채널 서버)
  case WM_CHAT_SOCKET_RECEIVE:   → ChatNetworkManager  (채팅 서버) ← 신규
```

---

## 3. 구현 단계

### Step 1. 전역 설정값 추가

`WinAPITest_Source/stbNetworkConfig.h`:

```cpp
namespace stb
{
    extern char g_CharacterId[32];
    extern int  g_ChannelPort;
    extern int  g_ChatPort;          // ← 추가

    struct NetworkConfig
    {
        static constexpr const char* SERVER_IP = "100.114.42.54";
        inline static int SERVER_PORT      = g_ChannelPort;
        inline static int CHAT_SERVER_PORT = g_ChatPort;    // ← 추가
        static const char* GetCharacterId() { return g_CharacterId; }
        static constexpr const char* MAP_ID = "100000000";
    };
}
```

CWorld 단계에서 채널 선택과 함께 채팅 서버 포트도 얻어와 `g_chat_port`에 세팅 →
`wWinMain`에서 `stb::g_ChatPort`로 복사 (이미 g_ChannelPort 처리한 패턴 동일).

### Step 2. ChatNetworkManager 추가 (별도 싱글톤)

`WinAPITest_Source/stbChatNetworkManager.h` (신규):

```cpp
#pragma once
#include "stbSingletonBase.h"
#include "MySocket.h"
#include "PacketParser.h"
#include <functional>
#include <map>

typedef struct HWND__* HWND;

namespace stb
{
    class ChatNetworkManager : public SingletonBase<ChatNetworkManager>
    {
    public:
        ChatNetworkManager();
        ~ChatNetworkManager();

        bool Connect(const std::string& ip, int port, HWND hWnd);
        void Disconnect();
        bool IsConnected() const { return m_bConnected; }

        // 채팅 전용 송신
        void SendChat(const std::string& msg);
        void SendChatInit(const std::string& charId);

        void SendPacket(uint16_t type, const std::vector<std::string>& datas);
        void ProcessReceivedData();

        using PacketHandler = std::function<void(const ParsedPacket&)>;
        void RegisterHandler(uint16_t type, PacketHandler handler);

    private:
        MySocket m_socket;
        bool m_bConnected = false;
        std::map<uint16_t, PacketHandler> m_handlers;
        void HandlePacket(const ParsedPacket&);
    };
}
```

핵심:
- **NetworkManager의 카피본**이라고 생각하면 됨.
- 내부 `MySocket m_socket` 은 NetworkManager의 소켓과 **완전 별개** → 동시에 살아 있어도 충돌 없음.
- `SetAsync(hWnd)` 시 알려줄 윈도우 메시지를 `WM_CHAT_SOCKET_RECEIVE` 로 바꿔야 함.

> ⚠️ `MySocket::SetAsync`는 현재 `WM_SOCKET_RECEIVE` 를 하드코딩해 두었을 수 있습니다.
> 그 경우, `SetAsync(hWnd, UINT message)` 로 메시지 ID를 파라미터로 받도록 작게 확장해야 합니다.
> (또는 ChatNetworkManager 전용 `SetAsyncChat()` 추가)

### Step 3. 채팅 패킷 타입 정의

`WinAPITest_Source/Packet.h` 의 enum에 다음을 정리:

```cpp
PKT_CHAT_INIT = 0x0003,   // 이미 존재
PKT_CHAT      = 0x0004,   // 이미 존재
// 필요 시:
// PKT_CHAT_BROADCAST  = 0x000B,
// PKT_CHAT_WHISPER    = 0x000C,
```

(현재 `Packet.h`에 `PKT_CHAT_INIT / PKT_CHAT`가 이미 정의돼 있으므로 그대로 활용)

### Step 4. ChatPacketHandler 추가

`WinAPITest_Source/ChatPacketHandler.h / .cpp` (신규):

```cpp
class ChatPacketHandler
{
public:
    static void Execute(const ParsedPacket& pkt);   // PKT_CHAT 수신 처리
    static void HandleChatInit(const ParsedPacket& pkt); // 인증 응답
    static void SendChatInit(const std::string& charId);
    static void SendChat(const std::string& msg);
};
```

`Execute`에서는 payload를 파싱(닉네임 + 메시지)해서 UI 메시지 큐에 push 하면 됨.

### Step 5. ChatScene 구현 (기존 스캐폴드 수정)

`WinAPITest_lib/stbChatScene.h`:

```cpp
#pragma once
#include "..\\WinAPITest_Source\\stbScene.h"

namespace stb
{
    class ChatScene : public Scene
    {
    public:
        ChatScene();
        ~ChatScene();

        void Initialize() override;
        void Update() override;
        void LateUpdate() override;
        void Render(HDC hdc) override;
        void Render(stbD2DRenderer& renderer) override;
        void OnEnter() override;
        void OnExit() override;

        // 채팅 UI 상태
        void ToggleInputFocus();           // Enter 키 등으로 토글
        bool IsInputActive() const { return mInputActive; }
        void AppendMessage(const std::wstring& line);

    private:
        bool mInputActive = false;
        std::wstring mInputBuffer;
        std::vector<std::wstring> mLogLines;   // 최근 N줄
    };
}
```

`WinAPITest_lib/stbChatScene.cpp` 의 `Initialize()`는
**PlayScene의 “플레이어/카메라/MonsterManager” 부분을 모두 제거**하고
**채팅 전용 초기화**만 남깁니다.

```cpp
void ChatScene::Initialize()
{
    Scene::Initialize();

    // 채팅용 네트워크 매니저 초기화
    HWND hWnd = stb::Application::getInstance()->GetHWND();

    // 핸들러 등록
    ChatNetworkManager::getInstance()->RegisterHandler(PKT_CHAT,
        [](const ParsedPacket& pkt){ ChatPacketHandler::Execute(pkt); });
    ChatNetworkManager::getInstance()->RegisterHandler(PKT_CHAT_INIT,
        [](const ParsedPacket& pkt){ ChatPacketHandler::HandleChatInit(pkt); });

    // 채팅 서버 접속 (PlayScene의 채널 서버와는 별개)
    ChatNetworkManager::getInstance()->Connect(
        NetworkConfig::SERVER_IP,
        NetworkConfig::CHAT_SERVER_PORT,
        hWnd);
}
```

> 🔑 **PlayScene 의 `Initialize()` 와 다른 점:**
> - Camera / Player / Animator / MonsterManager 초기화 코드는 **모두 빼야 함**
>   (이미 PlayScene이 만든 게임월드가 active scene으로 있으니까, 또 만들면 안 됨).
> - `UIManager::Init()` 도 PlayScene이 이미 호출했으므로 호출하지 않음.
> - **ChatNetworkManager** 를 사용한다는 점.

```cpp
void ChatScene::Update()
{
    Scene::Update();   // 채팅 Scene 내부 GameObject (있다면) 업데이트

    // 입력 처리 (예: Enter → 입력 포커스 토글)
    if (M_INPUT->GetKeyDown(eKeyCode::Enter))
        ToggleInputFocus();

    if (mInputActive)
    {
        // 텍스트 입력 처리 (간단히는 Input에서 키 이벤트로 mInputBuffer 갱신)
        if (M_INPUT->GetKeyDown(eKeyCode::Return) && !mInputBuffer.empty())
        {
            // UTF-16 → UTF-8 변환 후 송신
            std::string utf8 = WstrToUtf8(mInputBuffer);
            ChatPacketHandler::SendChat(utf8);
            mInputBuffer.clear();
        }
    }
}

void ChatScene::Render(stbD2DRenderer& renderer)
{
    Scene::Render(renderer);

    // 화면 하단에 반투명 박스 + 텍스트 라인들 + 입력박스 그리기
    DrawChatBackdrop(renderer);
    DrawChatLog(renderer, mLogLines);
    if (mInputActive) DrawChatInput(renderer, mInputBuffer);
}
```

### Step 6. SceneManager 확장 (덧그리기 슬롯 추가)

`WinAPITest_Source/stbSceneManager.h` 에 다음 추가:

```cpp
class SceneManager : public SingletonBase<SceneManager>
{
public:
    // ...
    void SetOverlayScene(const std::wstring& name);
    void ClearOverlayScene() { mOverlayScene = nullptr; }
private:
    Scene* mOverlayScene = nullptr;   // 신규
};
```

`stbSceneManager.cpp`:

```cpp
void SceneManager::SetOverlayScene(const std::wstring& name)
{
    auto it = mScenes.find(name);
    if (it != mScenes.end())
    {
        mOverlayScene = it->second;
        mOverlayScene->OnEnter();
    }
}

void SceneManager::Update()
{
    mActiveScene->Update();
    if (mOverlayScene) mOverlayScene->Update();
    mDontDestroyOnLoad->Update();
}

void SceneManager::Render(stbD2DRenderer& renderer)
{
    mActiveScene->Render(renderer);
    if (mOverlayScene) mOverlayScene->Render(renderer);
    mDontDestroyOnLoad->Render(renderer);
}
```

> 이렇게 하면 **active=PlayScene + overlay=ChatScene**이 동시에 매 프레임 동작하고,
> ChatScene이 항상 PlayScene 위에 그려집니다.

### Step 7. LoadScene 등록

`WinAPITest_lib/stbLoadScenes.h`:

```cpp
#include "stbPlayScene.h"
#include "stbChatScene.h"

namespace stb
{
    void LoadScene()
    {
        SCENEMANAGER->CreateScene<PlayScene>(L"PlayScene");
        SCENEMANAGER->CreateScene<ChatScene>(L"ChatScene");

        SCENEMANAGER->LoadScene(L"PlayScene");
        SCENEMANAGER->SetOverlayScene(L"ChatScene");   // ← 항상 덧대기
    }
}
```

### Step 8. WndProc에 채팅 소켓 메시지 분기 추가

`WinAPITest_Client/WinAPITest_Client.cpp` 의 `WndProc`:

```cpp
case WM_SOCKET_RECEIVE:           // 채널 서버 (기존)
    /* ... 기존 코드 그대로 ... */
    break;

case WM_CHAT_SOCKET_RECEIVE:      // 채팅 서버 (신규)
{
    int event = WSAGETSELECTEVENT(lParam);
    int error = WSAGETSELECTERROR(lParam);

    if (event == FD_READ && error == 0)
        ChatNetworkManager::getInstance()->ProcessReceivedData();
    else if (event == FD_CLOSE)
        ChatNetworkManager::getInstance()->Disconnect();
    else if (event == FD_CONNECT && error == 0)
        ChatPacketHandler::SendChatInit(NetworkConfig::GetCharacterId());
}
break;
```

그리고 `WM_CHAT_SOCKET_RECEIVE` 매크로는 `Packet.h`에:

```cpp
#define WM_SOCKET_RECEIVE      (WM_USER + 100)
#define WM_SOCKET_DISCONNECT   (WM_USER + 101)
#define WM_CHAT_SOCKET_RECEIVE (WM_USER + 102)   // ← 추가
```

`MySocket::SetAsync` 또는 ChatNetworkManager 쪽에서 `WSAAsyncSelect` 호출 시
이 메시지 ID로 등록해야 합니다.

---

## 4. 라이프사이클 정리

| 시점 | PlayScene | ChatScene | NetworkManager | ChatNetworkManager |
|---|---|---|---|---|
| `LoadScene()` (앱 시작) | Create + Active | Create + Overlay | (대기) | (대기) |
| PlayScene::Initialize | - | - | Connect (채널포트) | - |
| ChatScene::Initialize | - | 핸들러 등록 | - | Connect (채팅포트) |
| WM_SOCKET_RECEIVE | FD_CONNECT/READ 처리 | - | 작동 | - |
| WM_CHAT_SOCKET_RECEIVE | - | FD_CONNECT/READ 처리 | - | 작동 |
| WM_DESTROY | - | - | Disconnect | Disconnect ← 추가 잊지 말기 |

---

## 5. 함정 체크리스트

- [ ] `MySocket::SetAsync`가 `WM_SOCKET_RECEIVE`를 하드코딩하고 있다면,
       채팅용으로는 **다른 메시지 ID**로 등록되도록 확장해야 한다.
- [ ] `WSAStartup`은 NetworkManager 생성자에서 이미 호출됨.
       ChatNetworkManager 생성자에서 **또 호출해도 무해**(WSACleanup도 reference count).
       단, 둘 다 destructor에서 `WSACleanup`을 호출하므로 둘 다 살아있는 동안은 OK.
- [ ] ChatScene 안에서 **Camera / Player / MonsterManager를 다시 생성하지 않는다.**
       이미 active인 PlayScene이 들고 있다.
- [ ] `Scene::Initialize()`는 반드시 호출 (Scene 내부 Layer 셋업).
- [ ] ChatScene이 PlayScene의 입력(이동 키)을 가로채는 일이 없도록,
       채팅 입력 모드일 때만 키 입력을 흡수하도록 `mInputActive` 플래그를 잘 다룬다.
- [ ] 패킷 핸들러는 **ChatNetworkManager 쪽에 등록**한다.
       `PacketManager::RegisterAllHandlers()`는 채널용 NetworkManager에만 등록되어 있다.
- [ ] 채팅 서버 IP/포트는 채널 서버와 다를 수 있다.
       필요 시 `NetworkConfig::CHAT_SERVER_IP`도 따로 분리한다.
- [ ] DontDestroyOnLoad와 OverlayScene을 혼동하지 않는다.
       DontDestroyOnLoad는 “씬 전환에도 살아남는다”는 의미이고,
       OverlayScene은 “active 위에 덧그린다”는 의미.

---

## 6. 최소 동작 검증 시나리오

1. 빌드 후 클라이언트 두 개 실행 (캐릭터 ID 다르게).
2. 각 클라이언트에서 PlayScene 진입 → 채널 서버 접속 로그 확인.
3. ChatScene이 자동으로 overlay 등록되어 화면 하단에 채팅창 박스가 보이는지 확인.
4. Enter 입력 → 입력박스 활성화 → 메시지 타이핑 → Enter → `PKT_CHAT` 송신 로그 확인.
5. 다른 클라이언트에서 `PKT_CHAT` 수신 → `mLogLines`에 추가 → 화면에 출력되는지 확인.
6. ESC 또는 한번 더 Enter → 입력 포커스 해제 → PlayScene의 이동 키가 정상 동작하는지 확인.
7. 종료 시 `WM_DESTROY` 에서 두 매니저 모두 `Disconnect()` 호출되는지 확인.

---

## 7. 변형 A: SceneManager를 건드리지 않는 방법

SceneManager에 손을 대고 싶지 않다면:

- ChatScene을 `DontDestroyOnLoad` 자리에 “대신” 넣을 수는 없음
  (`DontDestroyOnLoad` 클래스 자체로 만들어져 있어 타입이 다름).
- 차선책: **ChatScene을 `mDontDestroyOnLoad`처럼 항상 살아있는 Scene으로 등록**하기 위해
  `SceneManager::Initialize()`에서 직접 `CreateScene<ChatScene>` 후
  `mDontDestroyOnLoad`와 동급으로 매 프레임 Update/Render만 해주는 보조 멤버를 추가.
- 또는 **PlayScene::Update / Render 안에서 ChatScene의 Update / Render를 직접 호출**.
  → PlayScene과 ChatScene의 결합이 강해지지만 SceneManager는 무수정.

---

## 8. 변형 B: 새 Scene 대신 ChatUI로 만드는 방법 (참고)

- `UIManager`에 `ChatUI*` 멤버를 추가하고, Init/Update/Render에 연결.
- 별도 서버 통신만 ChatNetworkManager로 분리하면 동일 효과.
- 장점: 기존 UI 패턴(InventoryUI, QuickSlotUI)과 100% 동일해서 학습/유지보수 편함.
- 단점: 사용자가 원하는 “ChatScene 클래스”라는 명시적 분리가 사라짐.

> 만약 이후에 “채팅창이 단순한 박스가 아니라, 채팅 전용 GameObject들이 많이 들어가는
> 별도 세계”가 된다면 Scene으로 두는 게 맞고, 단순 UI 한 개에 가까우면 UI로 두는 게 맞습니다.

---

## 9. 진행 순서 권장 (체크리스트)

1. [ ] `g_ChatPort` 전역 추가, CWorld·wWinMain에서 채팅 포트도 함께 세팅하도록 수정.
2. [ ] `Packet.h` 에 `WM_CHAT_SOCKET_RECEIVE` 매크로 추가.
3. [ ] `ChatNetworkManager` 작성 (NetworkManager 카피본 기반).
4. [ ] `MySocket::SetAsync` 가 메시지 ID를 받을 수 있도록 작은 확장.
5. [ ] `ChatPacketHandler` 작성 (SendChatInit / SendChat / Execute).
6. [ ] `stbChatScene.cpp` Initialize를 채팅 전용으로 다시 작성 (Player/Camera 제거).
7. [ ] `SceneManager`에 `SetOverlayScene` 추가.
8. [ ] `stbLoadScenes.h`에 ChatScene Create + Overlay 등록.
9. [ ] `WndProc`에 `WM_CHAT_SOCKET_RECEIVE` 분기 추가.
10. [ ] 두 클라이언트로 채팅 송수신 검증.
11. [ ] 종료 경로(`WM_DESTROY`)에서 채팅 매니저 Disconnect 보장.

---

## 10. 마지막 한 마디

- PlayScene을 카피해서 만든 현재의 `stbChatScene.cpp`는
  **PlayScene 코드가 거의 통째로 들어가 있어 같은 자원이 중복 생성**됩니다.
  꼭 Player / Camera / MonsterManager 초기화 부분을 비우고 시작하세요.
- 별도 서버 포트는 **무조건 NetworkManager 한 개로 해결 X.**
  싱글톤이라도 클래스를 하나 더 만들어서 “채팅 전용 회선”을 운영하는 게 정공법입니다.
- 학습 순서: PlayScene 흐름 100% 이해 → SceneManager 흐름 → MySocket/WSAAsyncSelect 흐름 →
  그 뒤에 ChatNetworkManager / ChatScene을 그 위에 얹는다.
