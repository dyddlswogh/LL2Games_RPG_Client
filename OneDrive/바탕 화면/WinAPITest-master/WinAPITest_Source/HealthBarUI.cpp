#include "HealthBarUI.h"
#include "stbD2DRenderer.h"
#include "stbTexture.h"
#include "stbResourceManager.h"
#include "PlayerManager.h"
#include <algorithm>

#define M_RESOURCEMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
#define M_PLAYERMANAGER stb::SingletonBase<PlayerManager>::getInstance()

void HealthBarUI::Init()
{
    m_background = M_RESOURCEMANAGER->Find<stb::Texture>(L"HealthBar_Background");
    m_hpLayer = M_RESOURCEMANAGER->Find<stb::Texture>(L"HealthBar_hpLayer");
    m_mpLayer = M_RESOURCEMANAGER->Find<stb::Texture>(L"HealthBar_mpLayer");
}


void HealthBarUI::Update()
{

}

void HealthBarUI::Render(stbD2DRenderer& renderer)
{
    RenderBackGround(renderer);
    RenderHpLayer(renderer);
    RenderMpLayer(renderer);
}


void HealthBarUI::RenderBackGround(stbD2DRenderer& renderer)
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
    float scaleX = rtSize.width / 1366.0f;
    float scaleY = rtSize.height / 768.0f;
    float scale = min(scaleX, scaleY);

    int drawWidth = (int)(bitmap->GetSize().width * scale);
    int drawHeight = (int)(bitmap->GetSize().height * scale);

    m_UIRect = UILayout::CalcRect(
        (int)rtSize.width,
        (int)rtSize.height,
        drawWidth,
        drawHeight,
        UIAnchor::CenterBottom,
        10,
        15
    );

    renderer.DrawBitmap(
        bitmap,
        (FLOAT)m_UIRect.x,
        (FLOAT)m_UIRect.y,
        (FLOAT)m_UIRect.width,
        (FLOAT)m_UIRect.height,
        1.0f
    );


}

void HealthBarUI::RenderHpLayer(stbD2DRenderer& renderer)
{
    auto player = M_PLAYERMANAGER->GetLocalPlayer();
    if (player == nullptr)
        return;

    ID2D1Bitmap* bitmap = m_hpLayer->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("bitmap null\n");
        return;
    }

    D2D1_SIZE_F rtSize = renderer.GetRenderTargetSize();

    float scaleX = rtSize.width / 1366.0f;
    float scaleY = rtSize.height / 768.0f;
    float scale = min(scaleX, scaleY);

    D2D1_SIZE_F bmpSize = bitmap->GetSize();

    int drawWidth = (int)(bmpSize.width * scale);
    int drawHeight = (int)(bmpSize.height * scale);

    // UI 위치 계산
    m_UIRect = UILayout::CalcRect(
        (int)rtSize.width,
        (int)rtSize.height,
        drawWidth,
        drawHeight,
        UIAnchor::CenterBottom,
        10,
        38
    );

    int curHp = player->GetStat()->GetCurHp();
    int maxHp = player->GetStat()->GetMaxHp();

    float hpRatio = 0.0f;
    if (maxHp > 0)
    {
        hpRatio = (float)curHp / (float)maxHp;
    }
    hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);

 
    float hpDrawWidth = m_UIRect.width * hpRatio;

    // 출력 영역
    D2D1_RECT_F destRect = D2D1::RectF(
        (FLOAT)m_UIRect.x,
        (FLOAT)m_UIRect.y,
        (FLOAT)m_UIRect.x + hpDrawWidth,
        (FLOAT)m_UIRect.y + (FLOAT)m_UIRect.height
    );

    // 원본 이미지에서 잘라올 영역
    D2D1_RECT_F srcRect = D2D1::RectF(
        0.0f,
        0.0f,
        bmpSize.width * hpRatio,
        bmpSize.height
    );

    renderer.DrawBitmap(
        bitmap,
        destRect,
        srcRect,
        1.0f
    );

}
void HealthBarUI::RenderMpLayer(stbD2DRenderer& renderer)
{
    auto player = M_PLAYERMANAGER->GetLocalPlayer();
    if (player == nullptr)
        return;

    ID2D1Bitmap* bitmap = m_mpLayer->GetD2DBitmap();
    if (bitmap == nullptr)
    {
        OutputDebugStringA("bitmap null\n");
        return;
    }

    D2D1_SIZE_F rtSize = renderer.GetRenderTargetSize();

    float scaleX = rtSize.width / 1366.0f;
    float scaleY = rtSize.height / 768.0f;
    float scale = min(scaleX, scaleY);

    D2D1_SIZE_F bmpSize = bitmap->GetSize();

    int drawWidth = (int)(bmpSize.width * scale);
    int drawHeight = (int)(bmpSize.height * scale);

    // UI 위치 계산
    m_UIRect = UILayout::CalcRect(
        (int)rtSize.width,
        (int)rtSize.height,
        drawWidth,
        drawHeight,
        UIAnchor::CenterBottom,
        10,
        19
    );

    int curMp = player->GetStat()->GetCurMp();
    int maxMp = player->GetStat()->GetMaxMp();

    float hpRatio = 0.0f;
    if (maxMp > 0)
    {
        hpRatio = (float)curMp / (float)maxMp;
    }
    hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);


    float hpDrawWidth = m_UIRect.width * hpRatio;

    // 출력 영역
    D2D1_RECT_F destRect = D2D1::RectF(
        (FLOAT)m_UIRect.x,
        (FLOAT)m_UIRect.y,
        (FLOAT)m_UIRect.x + hpDrawWidth,
        (FLOAT)m_UIRect.y + (FLOAT)m_UIRect.height
    );

    // 원본 이미지에서 잘라올 영역
    D2D1_RECT_F srcRect = D2D1::RectF(
        0.0f,
        0.0f,
        bmpSize.width * hpRatio,
        bmpSize.height
    );

    renderer.DrawBitmap(
        bitmap,
        destRect,
        srcRect,
        1.0f
    );
}

void HealthBarUI::RenderHpText(stbD2DRenderer& renderer)
{

}

void HealthBarUI::RenderMpText(stbD2DRenderer& renderer)
{

}
