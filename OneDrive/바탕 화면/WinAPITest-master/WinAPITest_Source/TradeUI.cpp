#include "TradeUI.h"
#include "stbResourceManager.h"
#include "stbApplication.h"
#include "stbInput.h"
#include "InventoryManager.h"
#include "TradePacketHandler.h"
#include "StringConvert.h"
#include "stbD2DRenderer.h"

#define M_APP stb::SingletonBase<stb::Application>::getInstance()
#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()
#define M_INVENTORYMANAGER stb::SingletonBase<InventoryManager>::getInstance()
#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()


void TradeUI::Init()
{
	m_myName = Convert::Utf8ToWstr("myName"); //test
	m_myId = "1"; //test
	m_txtBackground = M_REMANAGER->Find<stb::Texture>(L"Trade_normal"); //배경

    //버튼
	m_txtConfirmNormal = M_REMANAGER->Find<stb::Texture>(L"Trade_button_confirm_normal"); //등록하기
	m_txtTradeNormal = M_REMANAGER->Find<stb::Texture>(L"Trade_button_trade_normal"); //교환하기
	m_txtTradeChecked = M_REMANAGER->Find<stb::Texture>(L"Trade_button_trade_checked"); //교환대기

    //레이어
	m_txtLayerConfirmMe = M_REMANAGER->Find<stb::Texture>(L"Trade_layer_confirm_me"); //교환대기 레이어

    
	mActive = false;
}


void TradeUI::Update()
{
	if (!mActive) return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(M_APP->GetHWND(), &pt);

	if (M_INPUT->GetKeyDown(stb::eKeyCode::LButton))
		HandleLMouseClick(pt.x, pt.y);
}

void TradeUI::Render(HDC hdc)
{

}

void TradeUI::Render(stbD2DRenderer& renderer)
{

	if (!mActive)
		return;

    if (m_cancelPopupActive) //교환 취소 팝업
    {
        RenderCancelPopUp(renderer);
        return;
    }

	// 1. 배경 출력
    RenderBackground(renderer);

    //교환 대기 레이어
    if (m_ConfirmLayerMe)
        RenderConfirmLayerMe(renderer);

    if (m_ConfirmLayerTarget)
        RenderConfirmLayerTarget(renderer);

    // 2. 버튼 출력
    RenderButton(renderer);

	// 3. 배경 위에 닉네임 출력
	RenderNickname(renderer);
}

void TradeUI::RenderBackground(stbD2DRenderer& renderer)
{
    //stb::Texture* background = m_isExpand ? m_fullBackground : m_txtBackground;
    stb::Texture* background = m_txtBackground;
    if (background == nullptr)
    {
        OutputDebugStringA("mBackground null\n");
        return;
    }

    ID2D1Bitmap* bitmap = background->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("bitmap null\n");
        return;
    }

    D2D1_SIZE_F size = bitmap->GetSize();

    renderer.DrawBitmap(bitmap,
        (FLOAT)m_posX,
        (FLOAT)m_posY,
        size.width,
        size.height,
        1.0f);
}

void TradeUI::RenderConfirmLayerMe(stbD2DRenderer& renderer)
{
    {
        stb::Texture* confirmLayer = m_txtLayerConfirmMe;
        if (confirmLayer == nullptr)
        {
            OutputDebugStringA("m_txtLayerConfirmMe null\n");
            return;
        }

        ID2D1Bitmap* bitmap = confirmLayer->GetD2DBitmap();
        if (bitmap == nullptr)
        {
            OutputDebugStringA("bitmap null\n");
            return;
        }

        D2D1_SIZE_F size = bitmap->GetSize();

        renderer.DrawBitmap(bitmap,
            (FLOAT)m_posX + LAYER_CONFIRM_ME_X,
            (FLOAT)m_posY + LAYER_CONFIRM_ME_Y,
            size.width,
            size.height,
            1.0f);
    }
}
void TradeUI::RenderConfirmLayerTarget(stbD2DRenderer& renderer)
{
    {
        stb::Texture* confirmLayer = m_txtLayerConfirmMe;
        if (confirmLayer == nullptr)
        {
            OutputDebugStringA("m_txtLayerConfirmMe null\n");
            return;
        }

        ID2D1Bitmap* bitmap = confirmLayer->GetD2DBitmap();
        if (bitmap == nullptr)
        {
            OutputDebugStringA("bitmap null\n");
            return;
        }

        D2D1_SIZE_F size = bitmap->GetSize();

        renderer.DrawBitmap(bitmap,
            (FLOAT)m_posX + LAYER_CONFIRM_TARGET_X,
            (FLOAT)m_posY + LAYER_CONFIRM_TARGET_Y,
            size.width,
            size.height,
            1.0f);
    }
}

void TradeUI::RenderButton(stbD2DRenderer& renderer)
{
#if 1 //등록버튼
    {
        stb::Texture* confirm = m_txtConfirmNormal;
        if (confirm == nullptr)
        {
            OutputDebugStringA("m_txtConfirmNormal null\n");
            return;
        }

        ID2D1Bitmap* bitmap = confirm->GetD2DBitmap();
        if (bitmap == nullptr)
        {
            OutputDebugStringA("bitmap null\n");
            return;
        }

        D2D1_SIZE_F size = bitmap->GetSize();

        renderer.DrawBitmap(bitmap,
            (FLOAT)m_posX + BUTTON_CONFIRM_X,
            (FLOAT)m_posY + BUTTON_CONFIRM_Y,
            size.width,
            size.height,
            1.0f);
    }
#endif

#if 1 //교환버튼
    {
        stb::Texture* trade = m_ConfirmLayerMe ? m_txtTradeChecked : m_txtTradeNormal;
        if (trade == nullptr)
        {
            OutputDebugStringA("m_txtTradeNormal null\n");
            return;
        }

        ID2D1Bitmap* bitmap = trade->GetD2DBitmap();
        if (bitmap == nullptr)
        {
            OutputDebugStringA("bitmap null\n");
            return;
        }

        D2D1_SIZE_F size = bitmap->GetSize();

        renderer.DrawBitmap(bitmap,
            (FLOAT)m_posX + BUTTON_TRADE_X,
            (FLOAT)m_posY + BUTTON_TRADE_Y,
            size.width,
            size.height,
            1.0f);
    }
#endif
}

void TradeUI::RenderNickname(stbD2DRenderer& renderer)
{
    ID2D1RenderTarget* rt = renderer.GetRenderTarget();
    IDWriteFactory* writeFactory = renderer.GetWriteFactory();

    if (rt == nullptr || writeFactory == nullptr)
        return;

    IDWriteTextFormat* textFormat = nullptr;
    HRESULT hr = writeFactory->CreateTextFormat(
        L"맑은 고딕",
        nullptr,
        DWRITE_FONT_WEIGHT_BOLD,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        13.0f,
        L"ko-kr",
        &textFormat
    );

    if (FAILED(hr) || textFormat == nullptr)
        return;

    textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    ID2D1SolidColorBrush* brush = nullptr;
    hr = rt->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &brush
    );

    if (FAILED(hr) || brush == nullptr)
    {
        textFormat->Release();
        return;
    }

    // 왼쪽 타겟 닉네임 영역
    D2D1_RECT_F targetNickRect = D2D1::RectF(
        m_posX + 70.0f,
        m_posY + 151.0f,
        m_posX + 182.0f,
        m_posY + 173.0f
    );

    // 오른쪽 내 닉네임 영역
    D2D1_RECT_F myNickRect = D2D1::RectF(
        m_posX + 275.0f,
        m_posY + 151.0f,
        m_posX + 387.0f,
        m_posY + 173.0f
    );

    rt->DrawTextW(
        m_targetName.c_str(),
        static_cast<UINT32>(m_targetName.length()),
        textFormat,
        targetNickRect,
        brush
    );

    rt->DrawTextW(
        m_myName.c_str(),
        static_cast<UINT32>(m_myName.length()),
        textFormat,
        myNickRect,
        brush
    );

    brush->Release();
    textFormat->Release();
}
void TradeUI::RenderCancelPopUp(stbD2DRenderer& renderer)
{
    const float popupW = 360.f;
    const float popupH = 160.f;

    D2D1_SIZE_F rtSize = renderer.GetRenderTargetSize();

    float px = rtSize.width / 2.f - popupW / 2.f;
    float py = rtSize.height / 2.f - popupH / 2.f;

    // 팝업 배경
    renderer.FillRect(px, py, popupW, popupH,
        D2D1::ColorF(0.f, 0.f, 0.f, 0.85f));

    // 테두리
    renderer.DrawRect(px, py, popupW, popupH,
        D2D1::ColorF(D2D1::ColorF::Yellow), 2.f);

    // 메시지
    std::wstring message = L"'" + m_targetName + L"'님이 교환신청을 취소 하셨습니다.";

    D2D1_RECT_F msgRect = D2D1::RectF(
        px + 20.f,
        py + 30.f,
        px + popupW - 20.f,
        py + 70.f
    );

    renderer.DrawTextString(message, msgRect,
        D2D1::ColorF(D2D1::ColorF::White));

    // 버튼 위치 저장
    const float btnW = 100.f;
    const float btnH = 36.f;
    const float btnY = py + 100.f;

    m_cancelCheckButtonRect = D2D1::RectF(
        px + 65.f,
        btnY,
        px + 65.f + btnW,
        btnY + btnH
    );

    // 확인 버튼
    renderer.FillRect(
        m_cancelCheckButtonRect.left,
        m_cancelCheckButtonRect.top,
        m_cancelCheckButtonRect.right - m_cancelCheckButtonRect.left,
        m_cancelCheckButtonRect.bottom - m_cancelCheckButtonRect.top,
        D2D1::ColorF(0.1f, 0.35f, 0.1f, 0.9f)
    );

    renderer.DrawRect(
        m_cancelCheckButtonRect.left,
        m_cancelCheckButtonRect.top,
        m_cancelCheckButtonRect.right - m_cancelCheckButtonRect.left,
        m_cancelCheckButtonRect.bottom - m_cancelCheckButtonRect.top,
        D2D1::ColorF(D2D1::ColorF::LightBlue),
        1.5f
    );

    D2D1_RECT_F acceptTextRect = D2D1::RectF(
        m_cancelCheckButtonRect.left,
        m_cancelCheckButtonRect.top + 7.f,
        m_cancelCheckButtonRect.right,
        m_cancelCheckButtonRect.bottom
    );

    renderer.DrawTextString(L"확인", acceptTextRect,
        D2D1::ColorF(D2D1::ColorF::White));
}


static bool IsPointInRect(int x, int y, const D2D1_RECT_F& rect)
{
    return x >= rect.left &&
        x <= rect.right &&
        y >= rect.top &&
        y <= rect.bottom;
}

bool TradeUI::IsPointInTradeReady(int x, int y)
{
    stb::Texture* confirm = m_txtConfirmNormal;
    if (confirm == nullptr)
    {
        OutputDebugStringA("m_txtConfirmNormal null\n");
        return false;
    }

    ID2D1Bitmap* bitmap = confirm->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("bitmap null\n");
        return false;
    }

    D2D1_SIZE_F size = bitmap->GetSize();

    return x >= m_posX + BUTTON_TRADE_X &&
        x <= m_posX + BUTTON_TRADE_X + size.width &&
        y >= m_posY + BUTTON_TRADE_Y &&
        y <= m_posY + BUTTON_TRADE_Y + size.height;
}

//내 슬롯 클릭 -> 아이템 교환창 등록
void TradeUI::HandleLMouseClick(int mouseX, int mouseY)
{
#if 1 //test 마우스 위치 출력
    {
        int posX = mouseX - m_posX;
        int posY = mouseY - m_posY;
        std::string sTmpX = "m_posX + " + std::to_string(posX) + "\n";
        std::string sTmpY = "m_posY + " + std::to_string(posY) + "\n";
        OutputDebugStringA(sTmpX.c_str());
        OutputDebugStringA(sTmpY.c_str());
    }
#endif

    //교환 취소 팝업
    if (m_cancelPopupActive)
    {
        if (IsPointInRect(mouseX, mouseY, m_cancelCheckButtonRect))
        {
            OutputDebugStringA("[TradeUI] Cancel Check clicked\n");
            CloseCancelPopup();
        }
        return;
    }

    //교환 준비
    if (IsPointInTradeReady(mouseX, mouseY))
    {
        OutputDebugStringA("[TradeUI] TRADE Ready Clicked\n");
        //SendTradeReady
        m_ConfirmLayerMe = true;
        //switchTradeReady
        TradePacketHandler::SendTradeReady(m_targetId);
        return;
    }

	int slotIdx = GetClickedMySlotIndex(mouseX, mouseY);
	if (slotIdx == -1) return;

	//인벤토리에서 해당 슬롯 아이템 가져오기
	//InventoryItemInfo* item = M_INVENTORYMANAGER->FindSlot(inventoryType, slotIdx);
	//if (item == nullptr) return;

	//TradeSlotInfo 만들어서 서버에 전송
	//TradeSlotInfo tradeSlot = { slotIdx, std::to_string(item->itemId), item->itemCount };
	//TradePacketHandler::SendTradeAddItem(tradeSlot);
}

int TradeUI::GetClickedMySlotIndex(int mouseX, int mouseY)
{
	return 0;
}

void TradeUI::StartTrade(const std::string& targetId, const std::string& targetName)
{
    m_targetId = targetId;
	m_targetName = Convert::Utf8ToWstr(targetName);

}

void TradeUI::CloseTradeUI()
{
    if (mActive)
    {
        TradePacketHandler::SendTradeCancel(Convert::WstrToUtf8(m_targetName));
        mActive = false;
        m_targetName.clear();
    }
}

void TradeUI::OnCancelPopUp()
{
    if (mActive)
    {
        m_cancelPopupActive = true; 
    }
}

void TradeUI::CloseCancelPopup()
{
    mActive = false;
    m_cancelPopupActive = false;

    m_targetName.clear();

    m_cancelCheckButtonRect = D2D1::RectF(0, 0, 0, 0);
}


void TradeUI::OnReady()
{
    if (mActive)
    {
        m_ConfirmLayerTarget = true;
    }
}