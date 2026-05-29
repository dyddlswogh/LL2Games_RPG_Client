# MP3 BGM 리소스 시스템 구현 가이드

이 문서는 MP3 BGM을 현재 리소스 시스템에 맞춰 구현하는 방법을 정리합니다.

목표는 단순히 `PlayScene`에서 MP3 파일 경로를 직접 재생하는 것이 아니라, 이미지처럼 시작 시점에 BGM 리소스를 한 번에 등록하고 필요한 곳에서 키로 찾아 쓰는 구조입니다.

```text
stb::LoadResource()
  Texture 전부 로드
  AudioClip 전부 로드

PlayScene::OnEnter()
  ResourceManager에서 AudioClip 찾기
  Play(loop = true)

PlayScene::OnExit()
  Stop()
```

## 1. 전체 구조

새로 추가할 클래스:

```text
WinAPITest-master/WinAPITest_Source/stbAudioClip.h
WinAPITest-master/WinAPITest_Source/stbAudioClip.cpp
```

`AudioClip`은 기존 `Texture`처럼 `Resource`를 상속합니다.

```cpp
class AudioClip : public Resource
{
public:
    AudioClip();
    ~AudioClip();

    HRESULT Load(const std::wstring& path) override;

    void Play(bool loop = false);
    void Stop();
    void Close();

private:
    std::wstring mAlias;
    bool mOpened;
};
```

MP3 재생은 Windows MCI API를 사용합니다.

## 2. MP3 파일 위치

아래 폴더를 만들고 MP3 파일을 넣습니다.

```text
WinAPITest-master/Resources/BGM/
```

예시:

```text
WinAPITest-master/Resources/BGM/Henesys.mp3
WinAPITest-master/Resources/BGM/Login.mp3
WinAPITest-master/Resources/BGM/Boss.mp3
```

코드에서는 아래처럼 상대 경로로 로드합니다.

```cpp
L"Resources\\BGM\\Henesys.mp3"
```

## 3. stbAudioClip.h 추가

파일:

```text
WinAPITest-master/WinAPITest_Source/stbAudioClip.h
```

내용:

```cpp
#pragma once
#include "stbResource.h"

namespace stb
{
    class AudioClip : public Resource
    {
    public:
        AudioClip();
        ~AudioClip();

        HRESULT Load(const std::wstring& path) override;

        void Play(bool loop = false);
        void Stop();
        void Close();

    private:
        std::wstring mAlias;
        bool mOpened;
    };
}
```

## 4. stbAudioClip.cpp 추가

파일:

```text
WinAPITest-master/WinAPITest_Source/stbAudioClip.cpp
```

내용:

```cpp
#include "stbAudioClip.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

namespace
{
    int g_audioClipAliasSeed = 0;

    void LogMciError(const wchar_t* prefix, MCIERROR error)
    {
        if (error == 0)
            return;

        wchar_t errorText[256] = {};
        mciGetErrorStringW(error, errorText, 256);

        OutputDebugStringW(prefix);
        OutputDebugStringW(errorText);
        OutputDebugStringW(L"\n");
    }
}

namespace stb
{
    AudioClip::AudioClip()
        : Resource(enums::eResourceType::AudioClip)
        , mAlias(L"")
        , mOpened(false)
    {
    }

    AudioClip::~AudioClip()
    {
        Close();
    }

    HRESULT AudioClip::Load(const std::wstring& path)
    {
        Close();

        mAlias = L"AudioClip_" + std::to_wstring(++g_audioClipAliasSeed);

        std::wstring command =
            L"open \"" + path + L"\" type mpegvideo alias " + mAlias;

        MCIERROR result = mciSendStringW(command.c_str(), nullptr, 0, nullptr);
        if (result != 0)
        {
            LogMciError(L"[AudioClip] open failed: ", result);
            mAlias.clear();
            mOpened = false;
            return S_FALSE;
        }

        mOpened = true;
        return S_OK;
    }

    void AudioClip::Play(bool loop)
    {
        if (!mOpened)
            return;

        Stop();

        std::wstring command = L"play " + mAlias;
        if (loop)
            command += L" repeat";

        MCIERROR result = mciSendStringW(command.c_str(), nullptr, 0, nullptr);
        LogMciError(L"[AudioClip] play failed: ", result);
    }

    void AudioClip::Stop()
    {
        if (!mOpened)
            return;

        std::wstring stopCommand = L"stop " + mAlias;
        mciSendStringW(stopCommand.c_str(), nullptr, 0, nullptr);

        std::wstring seekCommand = L"seek " + mAlias + L" to start";
        mciSendStringW(seekCommand.c_str(), nullptr, 0, nullptr);
    }

    void AudioClip::Close()
    {
        if (!mOpened)
            return;

        std::wstring command = L"close " + mAlias;
        mciSendStringW(command.c_str(), nullptr, 0, nullptr);

        mAlias.clear();
        mOpened = false;
    }
}
```

이 구현은 `Load()` 시점에 MP3를 MCI로 열어둡니다. 이후 씬에서는 파일 경로를 몰라도 `AudioClip` 포인터만 찾아서 `Play()`를 호출하면 됩니다.

## 5. 프로젝트에 파일 추가

새 파일은 Visual Studio 프로젝트에 포함되어야 빌드됩니다.

공유 프로젝트 파일:

```text
WinAPITest-master/WinAPITest_Source/WinAPITest_Source.vcxitems
```

`ClInclude` 목록에 추가:

```xml
<ClInclude Include="$(MSBuildThisFileDirectory)stbAudioClip.h" />
```

`ClCompile` 목록에 추가:

```xml
<ClCompile Include="$(MSBuildThisFileDirectory)stbAudioClip.cpp" />
```

필터 파일도 정리하고 싶으면 아래 파일에 추가합니다.

```text
WinAPITest-master/WinAPITest_Source/WinAPITest_Source.vcxitems.filters
```

예시:

```xml
<ClCompile Include="$(MSBuildThisFileDirectory)stbAudioClip.cpp">
  <Filter>Resource</Filter>
</ClCompile>

<ClInclude Include="$(MSBuildThisFileDirectory)stbAudioClip.h">
  <Filter>Resource</Filter>
</ClInclude>
```

## 6. winmm.lib 링크 확인

`mciSendStringW`는 `winmm.lib`가 필요합니다.

`stbAudioClip.cpp`에 아래 코드가 있으므로 대부분은 이걸로 됩니다.

```cpp
#pragma comment(lib, "winmm.lib")
```

그래도 링크 에러가 나면 클라이언트 프로젝트에 직접 추가합니다.

```text
WinAPITest_Client 속성
  링커
  입력
  추가 종속성
  winmm.lib
```

에러 예시:

```text
unresolved external symbol mciSendStringW
```

## 7. LoadResource에서 BGM 한 번에 로드

파일:

```text
WinAPITest-master/WinAPITest_lib/stbLoadResource.h
```

include 추가:

```cpp
#include "..\\WinAPITest_Source\\stbAudioClip.h"
```

`LoadResource()` 안에 BGM 로드를 추가합니다.

```cpp
// BGM 리소스
RESOURCEMANAGER->Load<AudioClip>(L"BGM_Henesys", L"Resources\\BGM\\Henesys.mp3");
RESOURCEMANAGER->Load<AudioClip>(L"BGM_Login", L"Resources\\BGM\\Login.mp3");
RESOURCEMANAGER->Load<AudioClip>(L"BGM_Boss", L"Resources\\BGM\\Boss.mp3");
```

이제 BGM도 이미지처럼 앱 시작 시점에 리소스 매니저에 등록됩니다.

## 8. PlayScene에서 꺼내 쓰기

파일:

```text
WinAPITest-master/WinAPITest_lib/stbPlayScene.h
```

private 영역에 BGM 포인터를 추가합니다.

```cpp
private:
    class Player* mPlayer;
    class Texture* mBackground;
    class AudioClip* mBGM;
```

파일:

```text
WinAPITest-master/WinAPITest_lib/stbPlayScene.cpp
```

include 추가:

```cpp
#include "stbAudioClip.h"
```

생성자 초기화:

```cpp
PlayScene::PlayScene()
    :mPlayer(nullptr)
    , mBackground(nullptr)
    , mBGM(nullptr)
{
}
```

`Initialize()`에서 리소스 찾기:

```cpp
mBGM = M_REMANAGER->Find<AudioClip>(L"BGM_Henesys");
```

추천 위치는 배경 텍스처를 찾는 코드 근처입니다.

```cpp
mBackground = M_REMANAGER->Find<Texture>(L"Henesys_ground_1");
mBGM = M_REMANAGER->Find<AudioClip>(L"BGM_Henesys");
```

`OnEnter()`에서 재생:

```cpp
void PlayScene::OnEnter()
{
    Scene::OnEnter();

    if (mBGM != nullptr)
        mBGM->Play(true);
}
```

`OnExit()`에서 정지:

```cpp
void PlayScene::OnExit()
{
    if (mBGM != nullptr)
        mBGM->Stop();

    Scene::OnExit();
}
```

## 9. 다른 씬에서 쓰는 방식

다른 씬도 같은 방식입니다.

```cpp
AudioClip* bgm = M_REMANAGER->Find<AudioClip>(L"BGM_Login");
if (bgm != nullptr)
    bgm->Play(true);
```

효과음도 같은 `AudioClip`으로 시작할 수 있습니다.

```cpp
AudioClip* hitSound = M_REMANAGER->Find<AudioClip>(L"SFX_Hit");
if (hitSound != nullptr)
    hitSound->Play(false);
```

다만 같은 효과음을 짧은 시간에 여러 번 겹쳐 재생해야 한다면 MCI alias 하나로는 한계가 있습니다. 그때는 같은 파일을 여러 `AudioClip` 인스턴스로 풀링하거나 XAudio2 같은 전용 오디오 API로 넘어가는 편이 좋습니다.

## 10. BGM 교체 시 주의점

씬 안에서 다른 BGM으로 바꿀 때는 이전 BGM을 먼저 멈춥니다.

```cpp
if (mBGM != nullptr)
    mBGM->Stop();

mBGM = M_REMANAGER->Find<AudioClip>(L"BGM_Boss");

if (mBGM != nullptr)
    mBGM->Play(true);
```

이 패턴이 여러 씬에서 반복되면 나중에 `AudioManager`를 추가합니다.

```text
AudioManager
  PlayBGM(key)
  StopBGM()
  PlaySFX(key)
```

초기에는 `AudioClip`만으로 충분하고, 중복 코드가 생기기 시작하면 `AudioManager`를 얹는 순서가 좋습니다.

## 11. 구현 후 체크리스트

- `stbAudioClip.h/.cpp`가 프로젝트에 포함되었는지
- `stbLoadResource.h`에서 `AudioClip` include가 추가되었는지
- `LoadResource()`에서 BGM들이 한 번에 로드되는지
- `PlayScene`에서 `Find<AudioClip>(L"BGM_Henesys")` 키가 정확한지
- `OnEnter()`에서 반복 재생되는지
- `OnExit()`에서 정지되는지
- 링크 에러가 나면 `winmm.lib`가 잡혀 있는지
- 디버그 출력에 `[AudioClip] open failed`가 찍히면 MP3 경로와 인코딩을 확인했는지

## 12. 현재 단계의 결론

이번 구조의 핵심은 이것입니다.

```cpp
RESOURCEMANAGER->Load<AudioClip>(L"BGM_Henesys", L"Resources\\BGM\\Henesys.mp3");

AudioClip* bgm = M_REMANAGER->Find<AudioClip>(L"BGM_Henesys");
bgm->Play(true);
```

이미지와 동일하게 "로드는 시작 시 한 번, 사용은 필요한 곳에서 키로 찾기" 흐름을 유지할 수 있습니다.
