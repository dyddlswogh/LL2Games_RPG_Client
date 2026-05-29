# 배경 이미지 및 BGM 추가 가이드

이 문서는 현재 브랜치에서 배경 이미지와 BGM을 추가할 때 따라갈 작업 순서를 정리한 가이드입니다.

현재 클라이언트의 실행 흐름은 아래와 같습니다.

```text
WinAPITest_Client.cpp
  APP->Initialize(...)
  stb::LoadResource()
  stb::LoadScene()

Application::Run()
  Update()
  Render()
    D2D BeginFrame
    Clear
    SceneManager::Render(renderer)
    D2D EndFrame
```

따라서 배경 이미지는 `stbLoadResource.h`에서 텍스처로 로드하고, `PlayScene::Render(stbD2DRenderer&)`에서 다른 오브젝트와 UI보다 먼저 그리면 됩니다.

## 1. 배경 이미지 먼저 넣기

### 목표

- 준비한 PNG를 `Resources` 아래에 둡니다.
- `LoadResource()`에서 배경 이미지를 로드합니다.
- `PlayScene`이 로드된 텍스처를 들고 있다가 매 프레임 가장 먼저 그립니다.

추천 파일명은 아래처럼 고정해두면 편합니다.

```text
WinAPITest-master/Resources/Background/PlayScene_Background.png
```

현재 창 기준 해상도는 `WinAPITest_Client.cpp`의 `InitInstance()`에서 `1366 x 768`로 잡혀 있습니다. 처음 넣는 배경 PNG도 가능하면 `1366 x 768`로 준비하면 스케일 이슈를 줄일 수 있습니다.

## 2. 폴더 만들고 PNG 넣기

Visual Studio 또는 탐색기에서 아래 폴더를 만듭니다.

```text
WinAPITest-master/Resources/Background/
```

그리고 준비한 PNG를 아래 경로에 넣습니다.

```text
WinAPITest-master/Resources/Background/PlayScene_Background.png
```

주의할 점:

- 경로는 실행 파일 기준 상대 경로로 로드됩니다.
- 기존 리소스들이 `Resources\\Mario.png`처럼 `WinAPITest-master/Resources` 기준으로 로드되고 있으므로 같은 방식으로 맞춥니다.
- 파일명 대소문자와 확장자를 코드와 동일하게 맞춥니다.

## 3. 리소스 로드 코드 추가

파일:

```text
WinAPITest-master/WinAPITest_lib/stbLoadResource.h
```

`LoadResource()` 초반 캐릭터 이미지 로드 근처에 배경 텍스처를 추가합니다.

```cpp
RESOURCEMANAGER->Load<Texture>(
    L"PlayScene_Background",
    L"Resources\\Background\\PlayScene_Background.png"
);
```

예시 위치:

```cpp
RESOURCEMANAGER->Load<Texture>(L"Player", L"Resources\\Mario.png");
RESOURCEMANAGER->Load<Texture>(L"Mario2", L"Resources\\Mario2.png");
RESOURCEMANAGER->Load<Texture>(
    L"PlayScene_Background",
    L"Resources\\Background\\PlayScene_Background.png"
);
```

`LoadResource()` 마지막에 이미 아래 코드가 있으므로, 새로 로드한 PNG도 Direct2D용 비트맵으로 같이 준비됩니다.

```cpp
stbD2DRenderer& renderer = stb::Application::getInstance()->GetRenderer();
RESOURCEMANAGER->LoadAllD2D(renderer);
```

## 4. PlayScene에 배경 텍스처 멤버 추가

파일:

```text
WinAPITest-master/WinAPITest_lib/stbPlayScene.h
```

private 영역에 배경 텍스처 포인터를 추가합니다.

```cpp
private:
    class Player* mPlayer;
    class Texture* mBackground;
```

현재 `stbPlayScene.h`가 `stbTexture.h`를 include하지 않으므로 `class Texture*`처럼 전방 선언 형태로 두면 됩니다.

## 5. 생성자에서 초기화

파일:

```text
WinAPITest-master/WinAPITest_lib/stbPlayScene.cpp
```

생성자 초기화 목록에 `mBackground(nullptr)`를 추가합니다.

```cpp
PlayScene::PlayScene()
    :mPlayer(nullptr)
    , mBackground(nullptr)
{
}
```

## 6. Initialize에서 배경 텍스처 찾기

`PlayScene::Initialize()` 안에서 `LoadResource()`로 등록한 키를 찾아 멤버에 저장합니다.

추천 위치는 카메라 생성 직후입니다.

```cpp
void PlayScene::Initialize()
{
    GameObject* camera = object::Instantiate<GameObject>(
        enums::eLayerType::Particle,
        Vector2(0.0f, 0.0f)
    );
    Camera* cameraComp = camera->AddComponent<Camera>();
    render::mainCamera = cameraComp;

    mBackground = M_REMANAGER->Find<Texture>(L"PlayScene_Background");

    mPlayer = object::Instantiate<Player>(enums::eLayerType::Player);
```

## 7. Render에서 가장 먼저 그리기

`PlayScene::Render(stbD2DRenderer& renderer)`를 아래처럼 수정합니다.

```cpp
void PlayScene::Render(stbD2DRenderer& renderer)
{
    if (mBackground != nullptr && mBackground->GetD2DBitmap() != nullptr)
    {
        D2D1_SIZE_F size = renderer.GetRenderTargetSize();
        renderer.DrawBitmap(
            mBackground->GetD2DBitmap(),
            0.0f,
            0.0f,
            size.width,
            size.height
        );
    }

    Scene::Render(renderer);
    M_UIMANAGER->Render(renderer);
    M_MONSTERAMANGER->Render(renderer);
}
```

이렇게 하면 배경이 전체 렌더 타겟 크기에 맞춰 그려지고, 플레이어/몬스터/UI는 그 위에 올라옵니다.

## 8. 빌드 확인

빌드 후 실행했을 때 확인할 것:

- 배경 PNG 경로가 맞는지
- 배경이 플레이어와 UI 뒤에 그려지는지
- PNG가 너무 작거나 비율이 다르면 늘어나 보이지 않는지
- 디버그 출력에 `[Texture] D2D 비트맵 로드 실패` 계열 로그가 없는지

배경이 안 보이면 우선 아래 순서로 봅니다.

1. `Resources\\Background\\PlayScene_Background.png` 경로가 실제 파일 위치와 같은지 확인합니다.
2. `stbLoadResource.h`의 리소스 키가 `L"PlayScene_Background"`로 정확한지 확인합니다.
3. `PlayScene::Initialize()`에서 같은 키로 `Find<Texture>()`를 호출했는지 확인합니다.
4. `LoadResource()` 마지막의 `LoadAllD2D(renderer)`보다 위에서 배경을 로드했는지 확인합니다.

## 9. 나중에 BGM 추가 방향

현재 코드에는 `AudioClip`, `AudioSource` enum은 있지만 실제 오디오 로딩/재생 클래스는 아직 보이지 않습니다. 그래서 BGM은 두 가지 방향 중 하나를 선택하면 됩니다.

### 간단한 1차 구현

WinAPI의 MCI 또는 `PlaySound`를 사용해서 `PlayScene::OnEnter()`에서 재생하고 `OnExit()`에서 정지합니다.

추천 파일 위치:

```text
WinAPITest-master/Resources/BGM/PlayScene_BGM.wav
```

`PlaySound`를 쓰면 WAV가 가장 단순합니다. MP3를 쓰고 싶으면 MCI 쪽이 낫습니다.

### 엔진식 구현

`AudioClip`, `AudioSource` 컴포넌트를 실제 클래스로 구현한 뒤 `ResourceManager`에 `AudioClip` 로딩을 연결합니다.

장점:

- 씬별 BGM 관리가 깔끔해집니다.
- 효과음과 볼륨 제어를 같은 구조로 묶을 수 있습니다.

단점:

- 지금 배경 이미지 작업보다 변경 범위가 큽니다.
- 리소스 매니저, 컴포넌트, 씬 생명주기까지 같이 손봐야 합니다.

이번 브랜치에서 빠르게 “배경 이미지 + BGM 확인”이 목표라면 먼저 `PlaySound` 또는 MCI로 BGM을 붙이고, 이후 오디오 시스템을 별도 브랜치에서 정리하는 흐름을 추천합니다.
