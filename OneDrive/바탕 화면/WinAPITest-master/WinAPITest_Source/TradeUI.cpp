#include "TradeUI.h"
#include "stbResourceManager.h"
#include "stbApplication.h"
#include "stbInput.h"
#include "InventoryManager.h"
#include "TradePacketHandler.h"
#include "StringConvert.h"
#include "stbD2DRenderer.h"
#include "stbNetworkConfig.h"
#include <string>
#include "Util.h"
#include "ItemDataManager.h"

#define M_APP stb::SingletonBase<stb::Application>::getInstance()
#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()
#define M_INVENTORYMANAGER stb::SingletonBase<InventoryManager>::getInstance()
#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
#define M_ITEMDATAMANAGER stb::SingletonBase<ItemDataManager>::getInstance()

void TradeUI::Init()
{
	//m_myName = Convert::Utf8ToWstr("myName"); //test
	//m_myId = "1"; //test
    m_myId = stb::NetworkConfig::GetCharacterId();
    m_myName = Convert::Utf8ToWstr(stb::NetworkConfig::GetCharacterName());
	m_txtBackground = M_REMANAGER->Find<stb::Texture>(L"Trade_normal"); //배경

    //버튼
	m_txtConfirmNormal = M_REMANAGER->Find<stb::Texture>(L"Trade_button_confirm_normal"); //등록하기
	m_txtTradeNormal = M_REMANAGER->Find<stb::Texture>(L"Trade_button_trade_normal"); //교환하기
	m_txtTradeChecked = M_REMANAGER->Find<stb::Texture>(L"Trade_button_trade_checked"); //교환대기

    //레이어
	m_txtLayerConfirmMe = M_REMANAGER->Find<stb::Texture>(L"Trade_layer_confirm_me"); //교환대기 레이어

    
	mActive = false;

    //인벤토리
    Init_InventoryButton();
    Init_InventoryTab();
    CreateSlots();
}

void TradeUI::Init_InventoryTab()
{
    m_tabs[(int)InventoryType::Equip] =
    {
        InventoryType::Equip,
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_equip_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_equip_selected"),
            m_equipTabRect
        },
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_equip_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_equip_selected"),
            m_fullEquipTabRect
        }
    };

    m_tabs[(int)InventoryType::Consume] =
    {
        InventoryType::Consume,
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_consume_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_consume_selected"),
            m_consumeTabRect
        },
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_consume_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_consume_selected"),
            m_fullConsumeTabRect
        }
    };

    m_tabs[(int)InventoryType::Etc] =
    {
        InventoryType::Etc,
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_etc_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_etc_selected"),
            m_etcTabRect
        },
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_etc_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_etc_selected"),
            m_fullEtcTabRect
        }
    };


    m_tabs[(int)InventoryType::Setup] =
    {
        InventoryType::Setup,
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_setup_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_setup_selected"),
            m_setupTabRect
        },
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_setup_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_setup_selected"),
            m_fullSetupTabRect
        }
    };


    m_tabs[(int)InventoryType::Cash] =
    {
        InventoryType::Cash,
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_cash_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_cash_selected"),
            m_cashTabRect
        },
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_cash_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_cash_selected"),
            m_fullCashTabRect
        }
    };

    m_tabs[(int)InventoryType::Cosmetic] =
    {
        InventoryType::Cosmetic,
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_cosmetic_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_cosmetic_selected"),
            m_cosmeticTabRect
        },
        {
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_cosmetic_normal"),
            M_REMANAGER->Find<stb::Texture>(L"Inventory_full_cosmetic_selected"),
            m_fullCosmeticTabRect
        }
    };

}

void TradeUI::Init_InventoryButton()
{
    m_fullButton = { InventoryButtonType::Expand, UIButtonState::Normal,
                    m_minimize_fullButton,
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_full_normal"),
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_full_mouseOver"),
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_full_pressed"),
                    TRUE,
                    TRUE };

    m_minButton = { InventoryButtonType::Reduce, UIButtonState::Normal,
                    m_minimize_minButton,
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_min_normal"),
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_min_mouseOver"),
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_min_pressed"),
                    TRUE,
                    TRUE };

    m_closeButton = { InventoryButtonType::Close, UIButtonState::Normal,
                    m_minimize_closeButton,
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_close_normal"),
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_close_mouseOver"),
                    M_REMANAGER->Find<stb::Texture>(L"Inventory_close_pressed"),
                    TRUE,
                    TRUE };

    m_currentType = InventoryType::Equip;
    OutputDebugStringA("Init_InventoryButton Success \n");
}

void TradeUI::CreateSlots()
{
    mSlots.clear();

    for (int index = 0; index < m_slotMaxCount; ++index)
    {
        int col = index % m_slotCols;
        int row = index / m_slotCols;

        InventorySlotUI slot;
        slot.slotIndex = index;
        slot.x = m_inventoryImgPosX + m_slotStartX + col * (m_slotWidth + m_slotgapX);
        slot.y = m_inventoryImgPosY + m_slotStartY + row * (m_slotHeight + m_slotgapY);
        slot.width = m_slotWidth;
        slot.height = m_slotHeight;

        m_slots.push_back(slot);
    }

    UpdateSlotEnableState();
}

void TradeUI::ClearSlots()
{
    for (auto& slot : m_slots)
    {
        slot.itemId = 0;
        slot.itemCount = 0;
    }
}

void TradeUI::UpdateSlotEnableState()
{
    int visibleSlotCount = m_isExpand ? 128 : 32;

    OutputDebugStringA(std::to_string(visibleSlotCount).c_str());
    OutputDebugStringA("\n");
    for (int i = 0; i < (int)m_slots.size(); ++i)
    {
        m_slots[i].isEnabled = (i < visibleSlotCount);
    }
}

void TradeUI::UpdateInventoryByType()
{

    Inventory* inventory = M_INVENTORYMANAGER->GetInventory((int)m_currentType);
    if (inventory == nullptr)
        return;

    const std::vector<InventoryItemInfo>& items = inventory->GetItemInfos();

    std::string DebugMsg;

    DebugMsg = "Inventory Type : " + std::to_string((int)m_currentType) + "\n";

    OutputDebugStringA(DebugMsg.c_str());

    ClearSlots();
    for (const auto& item : items)
    {
        int slotIndex = item.slotPos;

        if (slotIndex < 0 || slotIndex >= (int)m_slots.size())
            continue;

        m_slots[slotIndex].itemId = item.itemId;
        m_slots[slotIndex].itemCount = item.itemCount;

    }
}

void TradeUI::Update()
{
	if (!mActive) return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(M_APP->GetHWND(), &pt);

	if (M_INPUT->GetKeyDown(stb::eKeyCode::LButton))
		HandleLMouseClick(pt.x, pt.y);

    /*if (M_INPUT->GetKey(stb::eKeyCode::LButton))
    {
        HandleDragging(pt.x, pt.y);
    }

    if (M_INPUT->GetKeyUp(stb::eKeyCode::LButton))
    {
        HandleMouseUp();
    }*/
}

void TradeUI::Render(HDC hdc)
{

}

void TradeUI::Render(stbD2DRenderer& renderer)
{

	if (!mActive)
		return;

    if (m_myName.empty())
    {
        m_myId = stb::NetworkConfig::GetCharacterId();
        m_myName = Convert::Utf8ToWstr(stb::NetworkConfig::GetCharacterName());
    }

    if (m_cancelPopupActive) //교환 취소 팝업
    {
        RenderCancelPopUp(renderer);
        return;
    }

    if (m_successPopupActive) //교환 완료 팝업
    {
        RenderSuccessPopUp(renderer);
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

    // 4. 인벤토리 버튼 출력
    RenderInventoryMenuButtons(renderer);
    // 5. 인벤토리 아이템 이미지 그리기
    RenderInventorySlotItem(renderer);
    // 6. 인벤토리 버튼 이미지 그리기
    RenderInventoryButtons(renderer);
    // 7. 테스트용 슬롯들 테두리 그리기
    RenderInventoryTestSlots(renderer);
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
#ifdef __TEST
        TEST_OPACITY);
#else
        1.0f);
#endif
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
#ifdef __TEST
            TEST_OPACITY);
#else
            1.0f);
#endif
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
#ifdef __TEST
            TEST_OPACITY);
#else
            1.0f);
#endif
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
#ifdef __TEST
            TEST_OPACITY);
#else
            1.0f);
#endif
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
#ifdef __TEST
            TEST_OPACITY);
#else
            1.0f);
#endif
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
void TradeUI::RenderSuccessPopUp(stbD2DRenderer& renderer)
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
        D2D1::ColorF(D2D1::ColorF::SeaShell), 2.f);

    // 메시지
    std::wstring message = L"'" + m_targetName + L"'님과 교환이 완료되었습니다.";

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

    m_successCheckButtonRect = D2D1::RectF(
        px + 65.f,
        btnY,
        px + 65.f + btnW,
        btnY + btnH
    );

    // 확인 버튼
    renderer.FillRect(
        m_successCheckButtonRect.left,
        m_successCheckButtonRect.top,
        m_successCheckButtonRect.right - m_successCheckButtonRect.left,
        m_successCheckButtonRect.bottom - m_successCheckButtonRect.top,
        D2D1::ColorF(0.1f, 0.35f, 0.1f, 0.9f)
    );

    renderer.DrawRect(
        m_successCheckButtonRect.left,
        m_successCheckButtonRect.top,
        m_successCheckButtonRect.right - m_successCheckButtonRect.left,
        m_successCheckButtonRect.bottom - m_successCheckButtonRect.top,
        D2D1::ColorF(D2D1::ColorF::LightBlue),
        1.5f
    );

    D2D1_RECT_F acceptTextRect = D2D1::RectF(
        m_successCheckButtonRect.left,
        m_successCheckButtonRect.top + 7.f,
        m_successCheckButtonRect.right,
        m_successCheckButtonRect.bottom
    );

    renderer.DrawTextString(L"확인", acceptTextRect,
        D2D1::ColorF(D2D1::ColorF::White));
}


void TradeUI::RenderInventoryMenuButtons(stbD2DRenderer& renderer)
{
    for (int i = 0; i < (int)InventoryType::EnumEnd; i++)
    {
        const InventoryTabButton& tab = m_tabs[i];
        const InventoryTabVisual& view = m_isExpand ? tab.fullView : tab.normalView;

        stb::Texture* img = (m_currentType == tab.type) ? view.selected : view.normal;
        if (img == nullptr)
            continue;

        ID2D1Bitmap* bitmap = img->GetD2DBitmap();
        if (bitmap == nullptr)
            continue;

        D2D1_SIZE_F size = bitmap->GetSize();

        renderer.DrawBitmap(
            bitmap,
            (FLOAT)m_inventoryImgPosX + view.rect.left,
            (FLOAT)m_inventoryImgPosY + view.rect.top,
            size.width,
            size.height,
            1.0f
        );
    }
}
void TradeUI::RenderInventorySlotItem(stbD2DRenderer& renderer)
{
    for (const auto& slot : m_slots)
    {
        if (!slot.isEnabled)
            continue;

        if (slot.itemId == 0)
            continue;


        const ItemData* itemData = M_ITEMDATAMANAGER->FindItemData(slot.itemId);

        if (itemData == nullptr)
            continue;

        std::wstring key(itemData->resourceName.begin(), itemData->resourceName.end());
        stb::Texture* texture = M_REMANAGER->Find<stb::Texture>(key);
        if (texture == nullptr)
            continue;

        ID2D1Bitmap* bitmap = texture->GetD2DBitmap();
        if (bitmap == nullptr)
            continue;

        renderer.DrawBitmap(
            bitmap,
            slot.x,
            slot.y,
            slot.width,
            slot.height,
            1.0f
        );
    }


}
void TradeUI::RenderInventoryButtons(stbD2DRenderer& renderer)
{
    // 확장 이미지
    stb::Texture* full_button = GetCurrentImg(m_fullButton);
    if (full_button == nullptr)
    {
        OutputDebugStringA("full_button null\n");
        return;
    }

    ID2D1Bitmap* bitmap = full_button->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("full_button bitmap null\n");
        return;
    }

    D2D1_SIZE_F size = bitmap->GetSize();

    renderer.DrawBitmap(bitmap,
        (FLOAT)m_inventoryImgPosX + m_fullButton.size.left,
        (FLOAT)m_inventoryImgPosY + m_fullButton.size.top,
        size.width,
        size.height,
        1.0f);


    // 확장 이미지
    stb::Texture* min_button = GetCurrentImg(m_minButton);
    if (min_button == nullptr)
    {
        OutputDebugStringA("min_button null\n");
        return;
    }

    bitmap = min_button->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("min_button bitmap null\n");
        return;
    }

    size = bitmap->GetSize();

    renderer.DrawBitmap(bitmap,
        (FLOAT)m_inventoryImgPosX + m_minButton.size.left,
        (FLOAT)m_inventoryImgPosY + m_minButton.size.top,
        size.width,
        size.height,
        1.0f);


    // 닫기 이미지
    stb::Texture* close_button = GetCurrentImg(m_closeButton);
    if (close_button == nullptr)
    {
        OutputDebugStringA("close_button null\n");
        return;
    }

    bitmap = close_button->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("close_button bitmap null\n");
        return;
    }

    size = bitmap->GetSize();

    renderer.DrawBitmap(bitmap,
        (FLOAT)m_inventoryImgPosX + m_closeButton.size.left,
        (FLOAT)m_inventoryImgPosY + m_closeButton.size.top,
        size.width,
        size.height,
        1.0f);
}
void TradeUI::RenderInventoryTestSlots(stbD2DRenderer& renderer)
{
    for (const auto& slot : m_slots)
    {
        if (slot.isEnabled)
        {
            renderer.DrawRect(
                (float)slot.x,
                (float)slot.y,
                (float)slot.width,
                (float)slot.height,
                D2D1::ColorF::Black
            );
        }

    }

    // 테스트용 인벤토리 드래그 영역 그리기
    renderer.DrawRect(
        (float)m_inventoryClickRect.left,
        (float)m_inventoryClickRect.top,
        (float)(m_inventoryClickRect.right - m_inventoryClickRect.left),
        (float)(m_inventoryClickRect.bottom - m_inventoryClickRect.top),
        D2D1::ColorF::Black
    );
}

stb::Texture* TradeUI::GetCurrentImg(InventoryButton& buttons)
{
    switch (buttons.state)
    {
    case UIButtonState::Normal:
        return buttons.nomalImg;
    case UIButtonState::Hover:
        return buttons.hoverImg;
    case UIButtonState::Pressed:
        return buttons.pressedImg;
    default:
        return buttons.nomalImg;
    }
}

static bool IsPointInRect(const RECT& tabRect, int mouseX, int mouseY)
{

    return  mouseX >= tabRect.left &&
        mouseX < tabRect.right &&
        mouseY >= tabRect.top &&
        mouseY < tabRect.bottom;
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
        int posX = mouseX;
        int posY = mouseY;
        std::string sTmpX = "mouseX + " + std::to_string(posX) + "\n";
        std::string sTmpY = "mouseY + " + std::to_string(posY) + "\n";
        OutputDebugStringA(sTmpX.c_str());
        OutputDebugStringA(sTmpY.c_str());
    }

    if (0)
    {
        int posX = mouseX - m_posX;
        int posY = mouseY - m_posY;
        std::string sTmpX = "m_posX + " + std::to_string(posX) + "\n";
        std::string sTmpY = "m_posY + " + std::to_string(posY) + "\n";
        OutputDebugStringA(sTmpX.c_str());
        OutputDebugStringA(sTmpY.c_str());
    }
#endif

    //인벤토리
    if (HandleTabClick(mouseX, mouseY))
        return;

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

    //교환 완료 팝업
    if (m_successPopupActive)
    {
        if (IsPointInRect(mouseX, mouseY, m_successCheckButtonRect))
        {
            OutputDebugStringA("[TradeUI] Cancel Check clicked\n");
            CloseSuccessPopup();
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

bool TradeUI::HandleTabClick(int mouseX, int mouseY)
{
    int localX = mouseX - m_inventoryImgPosX;
    int localY = mouseY - m_inventoryImgPosY;

    for (int i = 0; i < (int)InventoryType::EnumEnd; i++)
    {
        const InventoryTabButton& tab = m_tabs[i];
        const InventoryTabVisual& view = m_isExpand ? tab.fullView : tab.normalView;

        if (IsPointInRect(view.rect, localX, localY))
        {
            m_currentType = tab.type;
            UpdateInventoryByType();
            return true;
        }
    }
    return false;
}

void TradeUI::HandleDragging(int mouseX, int mouseY)
{
    if (!m_isDragging)
        return;

   /* int inventoryClickWidth = m_isExpand ? m_fullInventoryClickWidth : m_inventoryClickWidth;
    int inventoryClickHeight = m_isExpand ? m_fullInventoryClickHeight : m_inventoryClickHeight;

    m_inventoryImgPosX = mouseX - m_dragOffsetX;
    m_inventoryImgPosY = mouseY - m_dragOffsetY;

    m_inventoryClickRect.left = m_inventoryImgPosX;
    m_inventoryClickRect.top = m_inventoryImgPosY;
    m_inventoryClickRect.right = m_inventoryImgPosX + inventoryClickWidth;
    m_inventoryClickRect.bottom = m_inventoryImgPosY + inventoryClickHeight;*/

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
        //TradePacketHandler::SendTradeCancel(Convert::WstrToUtf8(m_targetName));
        TradePacketHandler::SendTradeCancel(m_targetId);
        mActive = false;
        m_ConfirmLayerMe = false;
        m_ConfirmLayerTarget = false;
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

void TradeUI::OnSuccessPopUp()
{
    if (mActive)
    {
        m_successPopupActive = true;
    }
}

void TradeUI::CloseCancelPopup()
{
    mActive = false;
    m_cancelPopupActive = false;
    m_ConfirmLayerMe = false;
    m_ConfirmLayerTarget = false;
    m_targetName.clear();

    m_cancelCheckButtonRect = D2D1::RectF(0, 0, 0, 0);
}

void TradeUI::CloseSuccessPopup()
{
    mActive = false;
    m_successPopupActive = false;
    m_targetName.clear();
    m_ConfirmLayerMe = false;
    m_ConfirmLayerTarget = false;
    m_successCheckButtonRect = D2D1::RectF(0, 0, 0, 0);
}

void TradeUI::OnReady()
{
    if (mActive)
    {
        m_ConfirmLayerTarget = true;
    }
}