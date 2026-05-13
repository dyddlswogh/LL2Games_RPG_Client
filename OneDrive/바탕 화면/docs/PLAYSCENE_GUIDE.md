# PlayScene 학습 가이드

> 본 문서는 `WinAPITest_lib/stbPlayScene.h / stbPlayScene.cpp` 를 기준으로
> Scene이 어떤 흐름으로 동작하고, 네트워크/UI/플레이어 객체가 어떻게 결합되는지
> 학습할 수 있도록 정리한 문서입니다.

---

## 1. 전체 그림

```
wWinMain (WinAPITest_Client.cpp)
  ├─ CLogin           (로그인 다이얼로그, ID/PW)
  ├─ CWorld           (캐릭터/채널 선택, g_char_id / g_channel_port 세팅)
  ├─ APP->Initialize  (Renderer, Scene/Time/Collision/Input/Data Manager 초기화)
  └─ stb::LoadScene() (= SceneManager에 PlayScene 등록 + Load)
        │
        └─ PlayScene::Initialize()
              ├─ Camera 생성
              ├─ Player 객체 + Transform + Animator + PlayerScript
              ├─ UIManager::Init()
              ├─ Logger::Init()
              ├─ PacketManager::RegisterAllHandlers()
              ├─ InitializeNetworkDebug(hWnd)  ← 서버 TCP 접속
              └─ MonsterManager::Init()
```

`APP->Run()` 은 메시지 루프 안에서 매 프레임 `Update / Render`를 호출하고,
`SceneManager`가 활성 Scene(현재는 PlayScene)의 `Update / Render`를 다시 호출합니다.

---

## 2. Scene 시스템 (기반 클래스)

### `stb::Scene` (WinAPITest_Source/stbScene.h)

```cpp
class Scene : public Entity
{
public:
    virtual void Initialize();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render(HDC hdc);
    virtual void Render(stbD2DRenderer& renderer);

    virtual void OnExit();
    virtual void OnEnter();

    void AddGameObject(GameObject* gameObject, const enums::eLayerType layertype);
    Layer* GetLayer(enums::eLayerType type) { return mLayers[(UINT)type]; }
private:
    void CreateLayers();
    std::vector<Layer*> mLayers;
};
```

핵심 포인트:
- Scene은 **Layer 배열**(eLayerType 기반)을 들고 있고, 그 안에 GameObject가 들어갑니다.
- `Update/LateUpdate/Render`는 모두 Layer를 순회해서 그 안의 GameObject들을 업데이트/렌더합니다.
- PlayScene 같은 파생 클래스에서 `Scene::Initialize()`, `Scene::Update()`, `Scene::Render()`를
  반드시 호출해줘야 Scene 안의 Object들이 정상 동작합니다.

### `stb::SceneManager` (WinAPITest_Source/stbSceneManager.h/.cpp)

```cpp
template <typename T>
Scene* CreateScene(const std::wstring& name)
{
    T* scene = new T();
    scene->SetName(name);
    mActiveScene = scene;
    scene->Initialize();       // ← 여기서 Scene::Initialize 호출됨
    mScenes.insert({name, scene});
    return scene;
}

Scene* LoadScene(const std::wstring& name)
{
    if (mActiveScene) mActiveScene->OnExit();
    mActiveScene = mScenes[name];
    mActiveScene->OnEnter();
}

void Update()  { mActiveScene->Update();  mDontDestroyOnLoad->Update(); }
void Render(...) { mActiveScene->Render(...); mDontDestroyOnLoad->Render(...); }
```

여기서 꼭 기억할 것:
- **동시에 활성화되는 Scene은 두 개**: `mActiveScene` + `mDontDestroyOnLoad`.
- 즉 SceneManager는 “현재 Scene + 항상 살아있는 Scene”만 그려줍니다.
- 따라서 *PlayScene 위에 다른 Scene을 겹쳐 그리려면* 이 구조를 활용하거나,
  PlayScene 내부에서 UI/오브젝트로 그려야 합니다. (ChatScene 만들 때 핵심 포인트)

---

## 3. PlayScene 진입 흐름 한 줄씩 보기

`stbPlayScene.cpp`:

```cpp
void PlayScene::Initialize()
{
    // (1) 카메라 생성
    GameObject* camera = object::Instantiate<GameObject>(
        enums::eLayerType::Particle, Vector2(0.0f, 0.0f));
    Camera* cameraComp = camera->AddComponent<Camera>();
    render::mainCamera = cameraComp;
```

> 카메라 GameObject 하나를 Particle 레이어에 만들고, `render::mainCamera` 전역에 세팅.
> 이후 모든 렌더는 이 카메라 기준으로 좌표 변환됩니다.

```cpp
    // (2) 플레이어 생성
    mPlayer = object::Instantiate<Player>(enums::eLayerType::Player);
    Transform* tr = mPlayer->AddComponent<Transform>();
    int charId = atoi(stb::NetworkConfig::GetCharacterId());
    tr->SetPosition(Vector2(300.0f, 300.0f));

    PlayerScript* playerScript = mPlayer->AddComponent<PlayerScript>();
    playerScript->SetOwner(mPlayer);
    M_PLMANAGER->SetLocalPlayer(mPlayer);
```

> Player라는 GameObject 파생 클래스를 Player 레이어에 instantiate.
> Transform / PlayerScript 컴포넌트를 붙이고, `PlayerManager`의 LocalPlayer로 등록.
> 이 LocalPlayer가 키보드 입력으로 움직이는 “내 캐릭터”.

```cpp
    // (3) UI 초기화
    M_UIMANAGER->Init();
```

> 인벤토리/퀵슬롯/HP바/거래 UI 등을 UIManager가 일괄 생성.
> 이후 Update/Render도 UIManager가 묶어서 처리해 줍니다.

```cpp
    // (4) 애니메이션
    Texture* spartaTex = M_REMANAGER->Find<Texture>(L"Sparta");
    Animator* spartaAnim = mPlayer->AddComponent<Animator>();
    if (spartaTex)
    {
        spartaAnim->CreateAnimation(L"Run", spartaTex,
            Vector2(990.0f, 192.0f), Vector2(110.0f, 96.0f),
            Vector2::Zero, 4, 0.15f);
        spartaAnim->PlayAnimation(L"Run", true);
    }
    playerScript->SetFollowers(nullptr, nullptr);
```

> ResourceManager가 `LoadResource()` 단계에서 미리 읽어둔 `L"Sparta"` 텍스처를 찾아
> Animator에 Run 애니메이션으로 등록.

```cpp
    // (5) Base Scene 초기화 → Layer 안의 GameObject들에게 Initialize 전파
    Scene::Initialize();
```

> 파생 클래스에서 만든 GameObject들도 결국은 Scene::Initialize → Layer → GameObject 순으로
> Initialize가 호출되며 Component들이 초기화됩니다.

```cpp
    // (6) 로깅 + 네트워크
    stb::Logger::Init();
    LOG("========== Client Start (ID: "
        << stb::NetworkConfig::GetCharacterId() << ") ==========\n");

    HWND hWnd = stb::Application::getInstance()->GetHWND();
    M_PKMANAGER->RegisterAllHandlers();
    LOG("========== Network Connect Start ==========\n");
    stb::InitializeNetworkDebug(hWnd);

    // (7) 몬스터
    M_MONSTERAMANGER->Init();
}
```

> 네트워크 흐름이 핵심입니다.
> - `PacketManager::RegisterAllHandlers()` : 패킷 타입별 핸들러 람다를 NetworkManager에 등록
> - `InitializeNetworkDebug(hWnd)` : `NetworkManager::Connect(SERVER_IP, g_ChannelPort, hWnd)` 호출
> - 비동기 소켓이므로 실제 연결 성공은 `WM_SOCKET_RECEIVE` 메시지 안 `FD_CONNECT`에서 확인하고,
>   거기서 `ChannelInitPacketHandler::SendChannelAuth()`를 보냅니다.

---

## 4. Update / Render 루프

```cpp
void PlayScene::Update()
{
    Scene::Update();              // Layer → GameObject → Component::Update
    M_UIMANAGER->Update();        // 모든 UI Update
    M_MONSTERAMANGER->Update(M_TIME->GetDeltaTime());
}

void PlayScene::Render(stbD2DRenderer& renderer)
{
    Scene::Render(renderer);      // 게임월드(플레이어, 맵 등)
    M_UIMANAGER->Render(renderer);// UI는 게임 위에 덮어 그림
    M_MONSTERAMANGER->Render(renderer);
}
```

순서가 중요합니다.
- Scene::Render → UIManager::Render → MonsterManager::Render 순으로
  **나중에 그리는 게 위에 보입니다** (Direct2D는 painter’s algorithm).
- 즉 UI는 항상 게임월드 위에 그려진다는 의미.
- **ChatScene을 “위에 덧대는 창”으로 만들고 싶다면 이 “나중에 그리기” 원리를 그대로 활용**합니다.

---

## 5. 네트워크 파이프라인 (PlayScene 관점)

### 5-1. 설정값

`WinAPITest_Source/stbNetworkConfig.h`:
```cpp
extern char g_CharacterId[32];
extern int  g_ChannelPort;

struct NetworkConfig
{
    static constexpr const char* SERVER_IP = "100.114.42.54";
    inline static int SERVER_PORT = g_ChannelPort;  // 채널 선택 시 갱신
    static const char* GetCharacterId() { return g_CharacterId; }
    static constexpr const char* MAP_ID = "100000000";
};
```

- `g_CharacterId / g_ChannelPort`는 CWorld 단계에서 세팅된 뒤,
  `wWinMain`에서 `stb::g_CharacterId / stb::g_ChannelPort`로 복사됩니다.
- PlayScene은 이 값을 그대로 사용해서 채널 서버에 접속합니다.

### 5-2. 연결 → 인증 → 맵 입장 시퀀스

1. `InitializeNetworkDebug(hWnd)`
   → `NetworkManager::Connect(SERVER_IP, g_ChannelPort, hWnd)` (비동기)
2. 윈도우 메시지 `WM_SOCKET_RECEIVE` 에 `FD_CONNECT` 이벤트 도착
   → `ChannelInitPacketHandler::SendChannelAuth()`
   → `PKT_CHANNEL_AUTH` 송신
3. 서버 응답: `PKT_CHANNEL_AUTH` 수신
   → `PacketManager`에 등록된 핸들러가 `ChannelInitPacketHandler::Execute()` 호출
   → `SendEnterMap(charId, MAP_ID)` 자동 송신
4. 서버에서 `PKT_PLAYER_INFO / PKT_INVENTORY_* / PKT_MONSTER_SNAPSHOT` 등 패킷 수신
   → 각 핸들러에서 PlayerManager / InventoryManager / MonsterManager 갱신

### 5-3. 송신 측 (예: 이동)

`PlayerScript`의 `Update`에서 `stb::SendPlayerMove(x, y, speed)`(stbNetworkDebug.h)를 호출하면
`NetworkManager::SendPacket(PKT_PLAYER_MOVE, {...})` 으로 직렬화·전송됩니다.

### 5-4. 윈도우 메시지 처리 (WinAPITest_Client.cpp)

```cpp
case WM_SOCKET_RECEIVE:
    if (event == FD_READ)     NetworkManager::getInstance()->ProcessReceivedData();
    else if (event == FD_CLOSE) NetworkManager::getInstance()->Disconnect();
    else if (event == FD_CONNECT && error == 0)
        ChannelInitPacketHandler::SendChannelAuth();
```

> **포인트:** 현재 구조는 “하나의 NetworkManager(=하나의 소켓)”이 전부 처리하고,
> 윈도우 메시지 `WM_SOCKET_RECEIVE` 한 개로 송수신 이벤트를 받습니다.
> 즉 PlayScene이 사용하는 채널 서버 한 개와만 통신합니다.

---

## 6. 학습 체크리스트

- [ ] `PlayScene::Initialize()`의 7단계 흐름을 다이어그램 없이 외운다.
- [ ] `Scene::Initialize / Update / Render`가 Layer → GameObject → Component 까지
       어떤 순서로 전파되는지 코드를 직접 따라간다.
- [ ] `NetworkManager::Connect` → `WSAAsyncSelect` → `WM_SOCKET_RECEIVE`(FD_CONNECT/FD_READ/FD_CLOSE)
       흐름을 그림으로 그릴 수 있다.
- [ ] `PacketManager::RegisterAllHandlers()`에 등록된 패킷 종류와 호출되는 클래스를 매핑한다.
- [ ] `g_CharacterId / g_ChannelPort`가 어느 단계에서 세팅되고 어디서 읽히는지 추적한다.
- [ ] `UIManager`가 가지고 있는 UI들(InventoryUI, QuickSlotUI, HealthBarUI, TradeUI)이
       어떤 패킷/입력에 의해 켜지고 꺼지는지 본다.
- [ ] `PlayScene::Render`에서 그리는 순서가 화면 위/아래 결정에 어떤 영향을 주는지 본다.
- [ ] `SceneManager`가 동시에 활성화하는 Scene이 `mActiveScene` + `mDontDestroyOnLoad`
       두 개라는 것을 확인한다 (ChatScene 설계의 출발점).

---

## 7. 자주 헷갈리는 점

| 헷갈리는 점 | 사실 |
|---|---|
| `SCENEMANAGER->CreateScene<T>()` 만 호출하면 Update가 안 도는 거 아닌가? | `CreateScene` 안에서 `mActiveScene = scene;` 까지 해주므로 바로 활성화됩니다. |
| `Scene::Initialize()`는 한 번만 호출되나? | 네. SceneManager가 등록 시점에 호출하고, 이후엔 `OnEnter/OnExit`로만 토글합니다. |
| `mDontDestroyOnLoad`는 무엇? | 씬 전환에도 살아남는 항상 활성 Scene. 매 프레임 활성 씬과 같이 Update/Render됨. |
| 채널 서버는 어디서 “선택”되나? | `CWorld`에서 사용자가 채널을 선택하면 `g_channel_port`가 세팅되고, `wWinMain`에서 `stb::g_ChannelPort`로 복사됩니다. |
| 네트워크가 연결되는 시점은? | `PlayScene::Initialize` 내 `InitializeNetworkDebug(hWnd)`. 그 전에 `RegisterAllHandlers()`가 먼저 호출되어야 패킷이 안 새어 나갑니다. |

---

## 8. 다음 단계

PlayScene 흐름이 충분히 이해됐다면, 같은 구조로 ChatScene을 만드는 것은
**“Scene 클래스의 모양은 동일 + 네트워크 소켓은 별도 관리”** 가 핵심입니다.

→ [`CHATSCENE_GUIDE.md`](CHATSCENE_GUIDE.md) 로 이어집니다.
