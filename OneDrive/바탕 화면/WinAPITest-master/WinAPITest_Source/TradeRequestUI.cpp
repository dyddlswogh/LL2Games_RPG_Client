#include "TradeRequestUI.h"
#include "TradePacketHandler.h"
#include "stbApplication.h"
#include "TradeManager.h"
#include "StringConvert.h"
//#include "stbInput.h"
//#include "stbResourceManager.h"

//#define M_APP stb::SingletonBase<stb::Application>::getInstance()
//#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()
//#define M_INVENTORYMANAGER stb::SingletonBase<InventoryManager>::getInstance()
//#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()

#define M_TRADEMANAGER stb::SingletonBase<TradeManager>::getInstance()

void TradeRequestUI::Init()
{
    //m_background = M_REMANAGER->Find<stb::Texture>(L"Trade_normal");
    mActive = false;
}


void TradeRequestUI::Update()
{
    if (!mActive) return;

    //TODO
    if (!m_done) return; //입력 끝나지 않으면 행동없음

    //완료 TODO;
    m_targetPlayerId = Convert::WstrToUtf8(m_inputBuffer);
    TradePacketHandler::SendTradeRequest(m_targetPlayerId);

    m_done = false; //초기화
    this->CloseWindow();
}

void TradeRequestUI::Render(HDC hdc)
{
}

void TradeRequestUI::Render(stbD2DRenderer& renderer)
{
    D2D1_SIZE_F rtSize = renderer.GetRenderTargetSize();
    float cx = rtSize.width / 2.f - BOX_W / 2.f;
    float cy = rtSize.height / 2.f;

    // 라벨
    D2D1_RECT_F labelRect = D2D1::RectF(cx, cy - 28.f, cx + BOX_W, cy);
    renderer.DrawTextString(L"Trade Input Nick(Enter)",
        labelRect, D2D1::ColorF(D2D1::ColorF::LightGray));

    // 입력 박스 배경
    renderer.FillRect(cx, cy, BOX_W, BOX_H,
        D2D1::ColorF(0.f, 0.f, 0.f, 0.7f));

    // 테두리
    renderer.DrawRect(cx, cy, BOX_W, BOX_H,
        D2D1::ColorF(D2D1::ColorF::Yellow), 1.5f);

    // 입력 텍스트 + 커서
    std::wstring display = m_inputBuffer + L"_";
    D2D1_RECT_F textRect = D2D1::RectF(cx + 6.f, cy + 4.f, cx + BOX_W, cy + BOX_H);
    renderer.DrawTextString(display, textRect,
        D2D1::ColorF(D2D1::ColorF::Yellow));
}

void TradeRequestUI::CloseWindow()
{
    mActive = false;
    m_inputBuffer.clear();  // 닫을 때 입력 버퍼 초기화
}

void TradeRequestUI::OnPopUp(const TradeRequestInfo& info)
{
    { char szTemp[2560] = { 0, }; sprintf_s(szTemp, "[%s][%d] gunoo22_TEST reqId[%s]", __FUNCTION__, __LINE__, info.requesterId.c_str()); OutputDebugStringA(szTemp); }
    std::string ansi_requesterName = Convert::Utf8ToAnsi(info.requesterName);
    { char szTemp[2560] = { 0, }; sprintf_s(szTemp, "[%s][%d] gunoo22_TEST reqName[%s]", __FUNCTION__, __LINE__, ansi_requesterName.c_str()); OutputDebugStringA(szTemp); }
    //TODO
    //교환창 팝업
}

void TradeRequestUI::OnChar(wchar_t ch)
{
    if (!mActive) return;

    if (ch == L'\b')  // 백스페이스
    {
        if (!m_inputBuffer.empty())
            m_inputBuffer.pop_back();
    }
    else if (ch >= L' ' && (int)m_inputBuffer.size() < MAX_LEN)
    {
        m_inputBuffer += ch;
    }
}

void TradeRequestUI::OnKeyDown(WPARAM key)
{
    if (!mActive) return;

    if (key == VK_RETURN && !m_inputBuffer.empty())
        m_done = true;
}

void TradeRequestUI::Backspace()
{
    //if (key == VK_RETURN && !m_inputBuffer.empty())
      //  m_done = true;
}
