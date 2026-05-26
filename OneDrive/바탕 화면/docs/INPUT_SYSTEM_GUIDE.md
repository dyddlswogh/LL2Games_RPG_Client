# 입력 시스템 분석 가이드 (PlayerScript 기준)

> `stbInput.h / stbInput.cpp` 와 `stbPlayerScript.cpp` 를 기준으로
> 키보드 입력이 어떻게 처리되는지 분석한 문서입니다.
> ChatUI에서 Enter 키 감지 및 입력 충돌 방지를 구현하기 전에 반드시 읽어야 합니다.

---

## 1. 전체 구조

```
GetAsyncKeyState(VK_XXX)          ← Windows API (매 프레임 폴링)
        │
        ▼
Input::Update()
  └─ UpdateKeys(key)
       └─ CheckIsKeyDown(key.keyCode)
            └─ ASCII[(UINT)keyCode] → 가상 키코드 조회
        │
        ▼
Keys[i].State  (Down / Pressed / Up / None)
        │
        ├─ GetKeyDown(eKeyCode)    → State == Down  (누른 순간 1프레임만)
        ├─ GetKey(eKeyCode)        → State == Pressed (계속 누르는 동안)
        └─ GetKeyUp(eKeyCode)      → State == Up   (뗀 순간 1프레임만)
```

`GetAsyncKeyState`는 OS에게 "지금 이 키가 눌려 있냐"를 매 프레임 물어보는 방식입니다.
한글/특수문자처럼 **IME를 통해 조합되는 문자**는 이 방식으로 캡처할 수 없습니다.
(→ Chat 텍스트 입력에는 `WM_CHAR` 메시지를 써야 합니다. ChatUI 가이드 참고)

---

## 2. eKeyCode 와 ASCII[] 배열의 관계

`stbInput.h` 에서 `eKeyCode`는 단순히 **내부 인덱스 역할**만 합니다.
실제 Windows 가상 키코드(VK_XXX)와의 매핑은 `stbInput.cpp`의 `ASCII[]` 배열이 담당합니다.

```cpp
// stbInput.cpp
int ASCII[(UINT)eKeyCode::EnumsEnd] =
{
    'F1','F2',...                                         // F1~F12
    '1','2',...'0',                                       // 숫자키
    'Q','W','E',...                                       // 알파벳
    VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,                  // 방향키
    VK_LBUTTON, VK_MBUTTON, VK_RBUTTON, VK_SPACE,
    VK_LMENU, VK_LCONTROL,
    VK_RETURN, VK_RETURN,                                // Enter, Return (둘 다 같은 키)
};
```

인덱스 순서가 `eKeyCode` enum 순서와 1:1 대응되어야 합니다.
**새 키를 추가할 때는 enum에 추가한 순서와 동일한 위치에 ASCII[]에도 추가해야 합니다.**

### eKeyCode 전체 목록 및 인덱스

| 인덱스 | eKeyCode | VK 코드 |
|--------|----------|---------|
| 0~11 | F1~F12 | - |
| 12~21 | Num1~Num0 | '1'~'0' |
| 22~31 | Q~P | 'Q'~'P' |
| 32~40 | A~L | 'A'~'L' |
| 41~47 | Z~M | 'Z'~'M' |
| 48~51 | Left, Right, Up, Down | VK_LEFT 등 |
| 52~55 | LButton, MButton, RButton, SPACE | VK_LBUTTON 등 |
| 56 | LALT | VK_LMENU |
| 57 | LCONTROL | VK_LCONTROL |
| 58 | Enter | VK_RETURN |
| 59 | Return | VK_RETURN |
| 60 | **EnumsEnd** (벡터 크기 기준) | - |

---

## 3. 키 상태 전이도

```
               누름                  계속 누름
  None ──────────► Down ──────────► Pressed
                              │
                              │ 뗌
                              ▼
                             Up ──────────► None
                              뗌
```

`GetKeyDown`은 `Down` 상태일 때만 true → **1프레임만 감지**.
`GetKey`는 `Pressed` 상태일 때만 true → **누르고 있는 동안 계속** true.

PlayerScript의 이동은 `GetAction`(Pressed 포함)을 써서 부드러운 이동을 구현하고,
인벤토리 열기/채팅 활성화처럼 **토글**이 필요한 것은 `GetKeyDown`을 씁니다.

---

## 4. 액션 바인딩 시스템

`eKeyCode`와 `eActionCode` 사이의 매핑을 `KeyBindInfo`로 연결합니다.

```cpp
// stbInput.h
struct KeyBindInfo
{
    eBindType type;   // Action / Skill / Item / UI / QuickSlot
    int value;        // eActionCode 또는 skill/item ID
};
```

### 기본 바인딩 (CreateDefaultBindings)

| 키 | 액션 |
|---|---|
| Left, Right, Up, Down | MoveLeft/Right/Up/Down |
| LALT | Jump |
| LCONTROL | Attack |
| SPACE | Interact |
| I | Inventory |
| K | SkillWindow |

### 바인딩 흐름

```cpp
// PlayerScript::HandleInput()
KeyBindInfo bindInfo;
if (M_INPUT->GetPressedBind(bindInfo))  // 어떤 키든 눌린 바인딩이 있으면
    ExecuteBind(bindInfo);               // type에 따라 분기

// ExecuteBind
case eBindType::Action: ExecuteAction((eActionCode)bindInfo.value);
case eBindType::Skill:  /* SkillManager::UseSkill(...) */
case eBindType::UI:     /* UIManager 토글 */
```

`GetPressedBind`는 `Keys[0]~Keys[EnumsEnd-1]`를 루프로 순회하면서
**Down 상태이면서 바인딩이 등록된 키**를 찾아 `bindInfo`를 반환합니다.

---

## 5. PlayerScript::Update 전체 흐름

```cpp
void PlayerScript::Update()
{
    Idle();        // 이동(GetAction) + 이동 패킷 throttle
    HandleInput(); // 액션 바인딩(GetPressedBind)
}
```

### Idle() 이동 처리

```cpp
if (M_INPUT->GetAction(eActionCode::MoveRight))  // GetKey(Right) 내부 호출
    pos.x += 100.0f * deltaTime;

// 이동 시 throttle timer로 네트워크 패킷 30ms마다 송신
if (moved && mNetworkSendTimer >= NETWORK_SEND_INTERVAL)
    stb::SendPlayerMove(pos.x, pos.y, 100.0f);
```

`GetAction()`은 `m_keyBindings`를 순회해서 해당 액션에 바인딩된 키의 `GetKey` 상태를 확인합니다.
즉 `GetAction(MoveRight)` = `GetKey(eKeyCode::Right)`.

---

## 6. 새 키 바인딩 추가 방법

예: Enter 키로 채팅 토글을 PlayerScript에서 처리하고 싶을 때:

```cpp
// CreateDefaultBindings에 추가
m_keyBindings[eKeyCode::Enter] = { eBindType::UI, (int)eActionCode::ChatToggle };
```

```cpp
// eActionCode에 ChatToggle 추가 (stbInput.h)
ChatToggle,
```

```cpp
// ExecuteAction에 추가
case eActionCode::ChatToggle:
    UIManager::getInstance()->ToggleChatUI();
    break;
```

> 단, **ChatScene::Update()에서 이미 Enter 키를 처리한다면** 이 방식은 중복이 됩니다.
> 어디서 처리할지는 ChatUI 가이드의 "입력 충돌 처리" 섹션을 참고하세요.

---

## 7. GetAsyncKeyState의 한계 — 텍스트 입력에 쓰면 안 되는 이유

| 상황 | GetAsyncKeyState | WM_CHAR |
|---|---|---|
| Enter 키 눌림 감지 | ✅ (VK_RETURN) | ✅ |
| 알파벳 입력 ('a', 'b') | ⚠ (대소문자 구분 없음, 'A'로만 옴) | ✅ |
| 한글 입력 (IME 조합) | ❌ 불가 | ✅ |
| 백스페이스, Delete | ✅ (VK_BACK) | ✅ |
| Shift+숫자(특수문자) | ❌ 조합 인식 불가 | ✅ |

채팅창 입력버퍼(`mInputBuffer`)에 문자를 추가할 때는 반드시 `WM_CHAR` 메시지를 사용해야 합니다.
→ 이 처리는 `WndProc` → `UIManager` → `ChatUI` 경로로 내려줍니다.
→ 자세한 내용은 [`CHATUI_GUIDE.md`](CHATUI_GUIDE.md) 를 참고합니다.

---

## 8. 채팅 활성 중 플레이어 이동 차단 (입력 충돌)

채팅창이 활성화된 상태에서 방향키/공격 키가 그대로 동작하면 안 됩니다.
현재 `PlayerScript::Idle()`는 매 프레임 `GetAction`을 무조건 호출하므로,
채팅창 상태를 확인하는 가드가 필요합니다.

**권장 방식**: `UIManager`에 `IsInputFocused()` 추가:

```cpp
// UIManager.h
bool IsInputFocused() const { return m_chatUI && m_chatUI->IsInputActive(); }
```

```cpp
// PlayerScript::Idle() 시작 부분
if (M_UIMANAGER->IsInputFocused())
    return;  // 채팅 입력 중이면 이동/공격 처리 스킵
```

이 한 줄로 채팅 입력 모드일 때 플레이어 조작이 완전히 차단됩니다.
