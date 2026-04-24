#pragma once
#include "UI.h"
#include "stbTexture.h"
#include "UILayout.h"

class QuickSlotUI : public UI
{
public:

	void Init() override;
	void Update() override;
	void Render(stbD2DRenderer& renderer) override;

	void CreateSlotRect();
	int GetSlotIndexByPoint(int mouseX, int mouseY);

private:
	stb::Texture* m_background = nullptr;

	bool isReduce = false;

	std::vector<UIRect> m_slotRects;
	
	UIRect m_UIRect;

private:
	static constexpr float BASE_BG_WIDTH = 557.0f;
	static constexpr float BASE_BG_HEIGHT = 67.0f;

	// 슬롯 배치 원본 기준값
	static constexpr int SLOT_COUNT = 32;
	static constexpr int SLOT_COLS = 16;

	static constexpr float BASE_START_X = 1.0f;      // 첫 슬롯 시작 X
	static constexpr float BASE_START_Y = 1.0f;      // 첫 슬롯 시작 Y
	static constexpr float BASE_SLOT_WIDTH = 33.0f;  // 슬롯 하나 너비
	static constexpr float BASE_SLOT_HEIGHT = 33.0f; // 슬롯 하나 높이
	static constexpr float BASE_GAP_X = 5.0f;        // 슬롯 가로 간격
	static constexpr float BASE_GAP_Y = 3.0f;        // 슬롯 세로 간격
};

