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
	m_background = M_REMANAGER->Find<stb::Texture>(L"Trade_normal");
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

	//stb::Texture* background = m_isExpand ? m_fullBackground : m_background;
	stb::Texture* background = m_background;
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

	// 2. 배경 위에 닉네임 출력
	RenderNickname(renderer);
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
        m_posX + 73.0f,
        m_posY + 168.0f,
        m_posX + 185.0f,
        m_posY + 190.0f
    );

    // 오른쪽 내 닉네임 영역
    D2D1_RECT_F myNickRect = D2D1::RectF(
        m_posX + 278.0f,
        m_posY + 168.0f,
        m_posX + 390.0f,
        m_posY + 190.0f
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

//내 슬롯 클릭 -> 아이템 교환창 등록
void TradeUI::HandleLMouseClick(int mouseX, int mouseY)
{
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

void TradeUI::StartTrade(const std::string& targetName)
{
	m_targetName = Convert::Utf8ToWstr(targetName);

}
