# ChatUI 제작 가이드

> ChatScene 위에 렌더링되는 채팅창 UI를 만드는 가이드입니다.
> HealthBarUI / QuickSlotUI 등 기존 UI 패턴을 최대한 따릅니다.

선행 학습: [`INPUT_SYSTEM_GUIDE.md`](INPUT_SYSTEM_GUIDE.md)

---

## 0. 한 줄 결론

> **ChatUI를 `UI` 기반 클래스로 만들어 UIManager에 등록,
> Enter 감지는 `GetKeyDown`으로, 실제 문자 입력은 `WM_CHAR`로 처리합니다.**

---

## 1. 전체 구조

```
WndProc
  ├─ WM_CHAR ──────────────────────► UIManager::AppendInputChar(wchar_t)
  │                                       └─ ChatUI::AppendChar(wchar_t)
  │                                             └─ mInputBuffer += ch
  │
  ├─ WM_KEYDOWN(VK_BACK) ──────────► UIManager::HandleBackspace()
  │
  └─ WM_CHAT_SOCKET_RECEIVE(FD_READ) → ChatNetworkManager::ProcessReceivedData()
                                           └─ ChatPacketHandler::HandleChat(pkt)
                                                 └─ UIManager::AppendChatMessage(nick, msg)
                                                       └─ ChatUI::AddMessage(line)

매 프레임:
  ChatScene::Update()
    └─ GetKeyDown(Enter) → ChatUI->ToggleInputFocus()

  ChatScene::Render(renderer)
    └─ UIManager::Render(renderer)
         └─ ChatUI::Render(renderer)
              ├─ FillRect (반투명 배경)
              ├─ DrawTextString (메시지 로그 N줄)
              └─ DrawRect + DrawTextString (입력박스 + 커서)
```

---

## 2. ChatUI 클래스 설계

### 2-1. 헤더 (`WinAPITest_Source/ChatUI.h`)

```cpp
#pragma once
#include "UI.h"
#include "UILayout.h"
#include <vector>
#include <string>

class stbD2DRenderer;

class ChatUI : public UI
{
public:
    void Init() override;
    void Update() override;
    void Render(stbD2DRenderer& renderer) override;

    // 외부에서 호출
    void ToggleInputFocus();
    bool IsInputActive() const { return mInputActive; }

    void AppendChar(wchar_t ch);    // WM_CHAR에서 호출
    void Backspace();               // WM_KEYDOWN(VK_BACK)에서 호출
    void SubmitInput();             // Enter 전송 시

    void AddMessage(const std::wstring& nick, const std::wstring& msg);

private:
    void RenderBackground(stbD2DRenderer& renderer);
    void RenderLogLines(stbD2DRenderer& renderer);
    void RenderInputBox(stbD2DRenderer& renderer);

private:
    static constexpr int MAX_LOG_LINES = 8;     // 채팅 로그 최대 줄 수
    static constexpr float LOG_BOX_HEIGHT = 160.f; // 로그 영역 높이
    static constexpr float INPUT_BOX_HEIGHT = 28.f;

    bool mInputActive = false;
    std::wstring mInputBuffer;                   // 현재 타이핑 중인 문자열
    std::vector<std::wstring> mLogLines;         // 최근 MAX_LOG_LINES 개
    UIRect mLogRect{};
    UIRect mInputRect{};
};
```

### 2-2. 구현 (`WinAPITest_Source/ChatUI.cpp`)

**Init** — 위치 계산만:
```cpp
void ChatUI::Init()
{
    // mActive = false → 처음엔 로그만 보이고 입력박스는 숨김
    // (로그는 항상 보이도록 mActive = true로 둬도 됨)
}
```

**ToggleInputFocus**:
```cpp
void ChatUI::ToggleInputFocus()
{
    mInputActive = !mInputActive;
    if (!mInputActive)
        mInputBuffer.clear();  // 닫을 때 입력 버퍼 초기화
}
```

**AppendChar / Backspace / SubmitInput**:
```cpp
void ChatUI::AppendChar(wchar_t ch)
{
    if (!mInputActive) return;
    if (mInputBuffer.size() >= 100) return;  // 최대 길이 제한
    mInputBuffer += ch;
}

void ChatUI::Backspace()
{
    if (!mInputActive || mInputBuffer.empty()) return;
    mInputBuffer.pop_back();
}

void ChatUI::SubmitInput()
{
    if (mInputBuffer.empty()) return;

    // UTF-16 → UTF-8 변환 후 송신
    std::string utf8 = Convert::WstrToUtf8(mInputBuffer);
    ChatPacketHandler::SendChat(utf8);
    mInputBuffer.clear();

    ToggleInputFocus();   // 전송 후 입력 모드 종료
}
```

**AddMessage**:
```cpp
void ChatUI::AddMessage(const std::wstring& nick, const std::wstring& msg)
{
    std::wstring line = L"[" + nick + L"] " + msg;
    mLogLines.push_back(line);
    if ((int)mLogLines.size() > MAX_LOG_LINES)
        mLogLines.erase(mLogLines.begin());  // 오래된 줄 제거
}
```

---

## 3. Render 구현

`stbD2DRenderer`가 제공하는 API:
```cpp
FillRect(x, y, w, h, color)                         // 반투명 배경
DrawRect(x, y, w, h, color, stroke)                  // 테두리
DrawTextString(wstring, D2D1_RECT_F, color, false)   // 텍스트
```

### 3-1. 위치 계산 (UILayout 활용)

```cpp
// 화면 하단 왼쪽 기준으로 배치
D2D1_SIZE_F rtSize = renderer.GetRenderTargetSize();
float screenW = rtSize.width;
float screenH = rtSize.height;

const float boxW = 500.f;
const float marginLeft = 10.f;
const float marginBottom = 50.f;  // HP바 위

float logX = marginLeft;
float logY = screenH - marginBottom - LOG_BOX_HEIGHT - INPUT_BOX_HEIGHT;
float inputY = screenH - marginBottom - INPUT_BOX_HEIGHT;
```

### 3-2. 전체 Render

```cpp
void ChatUI::Render(stbD2DRenderer& renderer)
{
    D2D1_SIZE_F rtSize = renderer.GetRenderTargetSize();
    float screenH = rtSize.height;

    const float boxW     = 500.f;
    const float logH     = LOG_BOX_HEIGHT;
    const float inputH   = INPUT_BOX_HEIGHT;
    const float marginL  = 10.f;
    const float marginB  = 50.f;

    float logX    = marginL;
    float logY    = screenH - marginB - logH - inputH;
    float inputX  = marginL;
    float inputY  = screenH - marginB - inputH;

    // ── 로그 배경 (반투명 검정) ──
    renderer.FillRect(logX, logY, boxW, logH,
        D2D1::ColorF(0.f, 0.f, 0.f, 0.45f));

    // ── 로그 텍스트 ──
    float lineH = logH / MAX_LOG_LINES;
    for (int i = 0; i < (int)mLogLines.size(); ++i)
    {
        float ty = logY + i * lineH;
        D2D1_RECT_F rect = D2D1::RectF(logX + 4, ty, logX + boxW, ty + lineH);
        renderer.DrawTextString(mLogLines[i], rect,
            D2D1::ColorF(D2D1::ColorF::White));
    }

    // ── 입력박스 (입력 모드일 때만) ──
    if (mInputActive)
    {
        // 배경
        renderer.FillRect(inputX, inputY, boxW, inputH,
            D2D1::ColorF(0.f, 0.f, 0.f, 0.7f));
        // 테두리
        renderer.DrawRect(inputX, inputY, boxW, inputH,
            D2D1::ColorF(D2D1::ColorF::Yellow), 1.5f);

        // 입력 텍스트 + 커서
        std::wstring display = mInputBuffer + L"_";
        D2D1_RECT_F rect = D2D1::RectF(inputX + 4, inputY + 2,
                                        inputX + boxW, inputY + inputH);
        renderer.DrawTextString(display, rect,
            D2D1::ColorF(D2D1::ColorF::Yellow));
    }
    else
    {
        // 비활성 상태: 얇은 회색 테두리만
        renderer.DrawRect(inputX, inputY, boxW, inputH,
            D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.4f), 1.f);

        D2D1_RECT_F rect = D2D1::RectF(inputX + 4, inputY + 2,
                                        inputX + boxW, inputY + inputH);
        renderer.DrawTextString(L"Enter 키를 눌러 채팅", rect,
            D2D1::ColorF(0.6f, 0.6f, 0.6f, 0.6f));
    }
}
```

---

## 4. UIManager 연동

### 4-1. UIManager.h 에 ChatUI 추가

```cpp
#include "ChatUI.h"

class UIManager : public stb::SingletonBase<UIManager>
{
public:
    // ... 기존 메서드
    void ToggleChatInput();
    bool IsInputFocused() const;
    void AppendInputChar(wchar_t ch);
    void HandleBackspace();
    void AppendChatMessage(const std::wstring& nick, const std::wstring& msg);

private:
    ChatUI* m_chatUI = nullptr;
    // ... 기존 멤버
};
```

### 4-2. UIManager.cpp 에 구현

```cpp
// Init()에 추가:
m_chatUI = new ChatUI();
m_chatUI->Init();
mUIs.push_back(m_chatUI);

// 새 메서드들:
void UIManager::ToggleChatInput()
{
    if (m_chatUI) m_chatUI->ToggleInputFocus();
}
bool UIManager::IsInputFocused() const
{
    return m_chatUI && m_chatUI->IsInputActive();
}
void UIManager::AppendInputChar(wchar_t ch)
{
    if (m_chatUI) m_chatUI->AppendChar(ch);
}
void UIManager::HandleBackspace()
{
    if (m_chatUI) m_chatUI->Backspace();
}
void UIManager::AppendChatMessage(const std::wstring& nick, const std::wstring& msg)
{
    if (m_chatUI) m_chatUI->AddMessage(nick, msg);
}
```

---

## 5. WndProc에 WM_CHAR 추가

`WinAPITest_Client/WinAPITest_Client.cpp`:

```cpp
#include "UIManager.h"
#define M_UIMANAGER stb::SingletonBase<UIManager>::getInstance()

// WndProc에 추가:
case WM_CHAR:
{
    wchar_t ch = (wchar_t)wParam;
    if (ch == VK_RETURN)            // Enter는 ChatScene::Update에서 처리
        break;
    if (ch == VK_BACK)             // Backspace
    {
        M_UIMANAGER->HandleBackspace();
        break;
    }
    if (ch == 0x1B)                // ESC → 입력 모드 종료
    {
        M_UIMANAGER->ToggleChatInput();
        break;
    }
    if (ch >= 0x20)                // 출력 가능한 문자만 추가
        M_UIMANAGER->AppendInputChar(ch);
}
break;
```

> **⚠️ WM_CHAR vs WM_KEYDOWN**
> `WM_CHAR`는 키 하나를 **문자**로 변환한 결과를 줍니다.
> 한글 IME 조합 완료 시점에 조합된 글자 전체가 `wParam`으로 들어옵니다.
> 반드시 `WM_CHAR`를 쓰고, `GetAsyncKeyState`로 알파벳을 직접 읽어서 append하면 안 됩니다.

---

## 6. ChatScene::Update 에서 Enter 처리

현재 `stbChatScene.cpp`:

```cpp
void ChatScene::Update()
{
    Scene::Update();

    // Enter 키 → 입력 모드 토글 OR 전송
    if (M_INPUT->GetKeyDown(eKeyCode::Enter))
    {
        auto uiMgr = stb::SingletonBase<UIManager>::getInstance();
        if (uiMgr->IsInputFocused())
        {
            // 입력 모드 → Enter = 전송 후 닫기
            // SubmitInput은 ChatUI 내부에서 처리하므로 UIManager 경유
            uiMgr->SubmitChatInput();   // 아래에서 UIManager에 추가
        }
        else
        {
            // 비활성 → Enter = 채팅창 열기
            uiMgr->ToggleChatInput();
        }
    }
}
```

`UIManager`에 `SubmitChatInput()` 추가:
```cpp
void UIManager::SubmitChatInput()
{
    if (m_chatUI) m_chatUI->SubmitInput();
}
```

---

## 7. PlayerScript 이동 차단

`stbPlayerScript.cpp`의 `Idle()` 맨 앞:

```cpp
void PlayerScript::Idle()
{
    if (M_UIMANAGER->IsInputFocused())
        return;   // 채팅 입력 중 → 이동/공격 모두 차단

    // ... 기존 이동 코드
}
```

---

## 8. ChatPacketHandler에서 UIManager 호출

`ChatPacketHandler::HandleChat` 에서 메시지를 받아 UIManager에 전달:

```cpp
void ChatPacketHandler::HandleChat(const ParsedPacket& pkt)
{
    size_t offset = 0;
    const char* data = pkt.payload.c_str();
    size_t payloadSize = pkt.payload.size();
    std::string errMsg, nick, msg;

    if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, nick, errMsg)) return;
    if (!PacketParser::ParseLengthPrefixedString(data, payloadSize, offset, msg,  errMsg)) return;

    // UTF-8 → wstring 변환 후 UI에 전달
    std::wstring wNick = Convert::Utf8ToWstr(nick);
    std::wstring wMsg  = Convert::Utf8ToWstr(msg);

    UIManager::getInstance()->AppendChatMessage(wNick, wMsg);
}
```

---

## 9. 전체 진행 체크리스트

1. [ ] `WinAPITest_Source/ChatUI.h` 생성 (UI 상속)
2. [ ] `WinAPITest_Source/ChatUI.cpp` 생성 (Init/Update/Render/AppendChar/Backspace/SubmitInput/AddMessage)
3. [ ] `UIManager.h`에 ChatUI* 멤버 + 5개 메서드 선언 추가
4. [ ] `UIManager.cpp`에 ChatUI 생성/등록 + 5개 메서드 구현
5. [ ] `WndProc`에 `case WM_CHAR:` 추가
6. [ ] `ChatScene::Update()`에서 Enter 키 분기 처리
7. [ ] `PlayerScript::Idle()` 맨 앞에 `IsInputFocused()` 가드 추가
8. [ ] `ChatPacketHandler::HandleChat`에서 `UIManager::AppendChatMessage` 호출
9. [ ] 빌드 후 두 클라이언트로 채팅 테스트

---

## 10. 렌더 위치 기준 요약

```
화면 (1366 x 768 기준)
────────────────────────────────────────────────
│                                              │
│                  (게임 월드)                   │
│                                              │
│ [채팅 로그 박스 — 반투명]   ← 500 x 160       │
│ [Nick] 메시지 1                              │
│ ...                                          │
│ [입력 박스 — 28px]          ← 500 x 28        │
│──────────────────────────────────────────────│
│           [HP/MP 바]        ← CenterBottom    │
└──────────────────────────────────────────────┘
```

HP바 위쪽 `marginBottom = 50.f` 을 기준으로 쌓아 올리는 구조입니다.
HP바 위치가 바뀌면 `marginBottom`만 조정하면 됩니다.
