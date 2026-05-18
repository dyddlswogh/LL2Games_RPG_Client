#pragma once
#include "UI.h"
#include "stbTexture.h"
#include "Trade_Info.h"

class TradeRequestUI : public UI
{
public:
    void Init() override;
    void Update() override;
    void Render(HDC hdc) override;
    void Render(stbD2DRenderer& renderer) override;

public:
    void OnChar(wchar_t ch);      // WM_CHAR 연결
    void OnKeyDown(WPARAM key);   // WM_KEYDOWN 연결
    void Backspace();
    void CloseWindow();
    void OnPopUp(const TradeRequestInfo& info);

    bool        IsDone() const { return m_done; }
    std::wstring GetNickname() const { return m_inputBuffer; }

private:
    std::string m_targetPlayerId;
    std::wstring m_inputBuffer;
    bool         m_done = false;

    static constexpr float BOX_W = 300.f;
    static constexpr float BOX_H = 36.f;
    static constexpr int   MAX_LEN = 12;
};
