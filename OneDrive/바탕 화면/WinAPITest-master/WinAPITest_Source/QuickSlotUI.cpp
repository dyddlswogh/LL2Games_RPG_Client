#include "QuickSlotUI.h"
#include "stbTexture.h"
#include "stbD2DRenderer.h"
#include "stbResourceManager.h"
#include "UILayout.h"
#include "stbApplication.h"
#include "stbInput.h"

#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
#define M_APP stb::SingletonBase<stb::Application>::getInstance()
#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()

void QuickSlotUI::Init()
{
    m_background = M_REMANAGER->Find<stb::Texture>(L"quickslot_background");
    
}

void QuickSlotUI::Update()
{

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(M_APP->GetHWND(), &pt);

    if (M_INPUT->GetKeyDown(stb::eKeyCode::LButton))
    {
        GetSlotIndexByPoint(pt.x, pt.y);
    }

}

void QuickSlotUI::Render(stbD2DRenderer& renderer)
{
    
    if (m_background == nullptr)
    {
        OutputDebugStringA("m_background null\n");
        return;
    }

    ID2D1Bitmap* bitmap = m_background->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("bitmap null\n");
        return;
    }

    D2D1_SIZE_F rtSize = renderer.GetRenderTargetSize();

    // 기준 해상도 대비 UI 전체 스케일
    float scaleX = rtSize.width / 1000.0f;
    float scaleY = rtSize.height / 1000.0f;
    float scale = min(scaleX, scaleY);

    int drawWidth = (int)(BASE_BG_WIDTH * scale);
    int drawHeight = (int)(BASE_BG_HEIGHT * scale);

    m_UIRect = UILayout::CalcRect(
        (int)rtSize.width,
        (int)rtSize.height,
        drawWidth,
        drawHeight,
        UIAnchor::BottomRight,
        10,
        15
    );

    CreateSlotRect();

    renderer.DrawBitmap(
        bitmap,
        (FLOAT)m_UIRect.x,
        (FLOAT)m_UIRect.y,
        (FLOAT)m_UIRect.width,
        (FLOAT)m_UIRect.height,
        0.5f
    );

    for (const auto& rect : m_slotRects)
    {
        renderer.DrawRect(
            (FLOAT)rect.x,
            (FLOAT)rect.y,
            (FLOAT)rect.width,
            (FLOAT)rect.height,
            D2D1::ColorF::Black
        );
    }

    renderer.DrawRect(
        (FLOAT)m_UIRect.x,
        (FLOAT)m_UIRect.y,
        (FLOAT)m_UIRect.width,
        (FLOAT)m_UIRect.height,
        D2D1::ColorF::Black
    );
}

// 나중에 다시 맞춰야함
void QuickSlotUI::CreateSlotRect()
{
    m_slotRects.clear();
    m_slotRects.reserve(SLOT_COUNT);

    float scaleX = (float)m_UIRect.width / BASE_BG_WIDTH;
    float scaleY = (float)m_UIRect.height / BASE_BG_HEIGHT;

    int startX = m_UIRect.x + (int)(BASE_START_X * scaleX);
    int startY = m_UIRect.y + (int)(BASE_START_Y * scaleY);

    int slotWidth = (int)(BASE_SLOT_WIDTH * scaleX);
    int slotHeight = (int)(BASE_SLOT_HEIGHT * scaleY);

    int gapX = (int)(BASE_GAP_X * scaleX);
    int gapY = (int)(BASE_GAP_Y * scaleY);

    for (int i = 0; i < SLOT_COUNT; ++i)
    {
        int col = i % SLOT_COLS;
        int row = i / SLOT_COLS;

        UIRect rect;
        rect.x = startX + col * (slotWidth + gapX);
        rect.y = startY + row * (slotHeight + gapY);
        rect.width = slotWidth;
        rect.height = slotHeight;

        m_slotRects.push_back(rect);
    }
 }

int QuickSlotUI::GetSlotIndexByPoint(int mouseX, int mouseY)
{
    for (int i = 0; i < (int)m_slotRects.size(); ++i)
    {
        const UIRect& rect = m_slotRects[i];

        if (mouseX >= rect.x && mouseX <= rect.x + rect.width &&
            mouseY >= rect.y && mouseY <= rect.y + rect.height)
        {
            return i;
        }
    }

    return -1;
}

