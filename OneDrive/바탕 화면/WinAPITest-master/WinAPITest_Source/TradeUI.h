#pragma once
#include "UI.h"
#include "stbTexture.h"
#include "Trade_Info.h"
#include <d2d1.h>

class stbD2DRenderer;

class TradeUI : public UI
{
public:
	void Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Render(stbD2DRenderer& renderer) override;



private:
	//내 아이템 슬롯 영역(왼쪽 패널)
	void RenderMySlots(stbD2DRenderer& renderer);
	//상대 아이템 슬롯 영역
	void RenderTargetSlots(stbD2DRenderer& renderer);
	//확정/취소 버튼
	void RenderButtons(stbD2DRenderer& renderer);
	void RenderBackground(stbD2DRenderer& renderer);
	void RenderConfirmLayerMe(stbD2DRenderer& renderer);
	void RenderConfirmLayerTarget(stbD2DRenderer& renderer);
	void RenderButton(stbD2DRenderer& renderer);
	void RenderNickname(stbD2DRenderer& renderer);
	void RenderCancelPopUp(stbD2DRenderer& renderer);
	void RenderSuccessPopUp(stbD2DRenderer& renderer);


	bool IsPointInTradeReady(int mouseX, int mouseY);

	void HandleLMouseClick(int mouseX, int mouseY);
	int GetClickedMySlotIndex(int mouseX, int mouseY);
	void CloseCancelPopup();
	void CloseSuccessPopup();



public:
	void StartTrade(const std::string& targetId, const std::string& targetName);
	void CloseTradeUI();
	void OnCancelPopUp(); //상대가 교환 취소했다는 팝업
	void OnSuccessPopUp(); //교환 완료 팝업
	void OnReady(); //상대 교환 준비



private:
	bool         m_cancelPopupActive = false; //상대 교환 취소 팝업용
	bool         m_successPopupActive = false; //상대 교환 완료 팝업용
	D2D1_RECT_F m_cancelCheckButtonRect{};   //교환취소 확인버튼
	D2D1_RECT_F m_successCheckButtonRect{};   //완료 확인버튼

	bool         m_ConfirmLayerMe= false; //교환 대기 레이어
	bool         m_ConfirmLayerTarget= false; //교환 대기 레이어
	
	stb::Texture* m_txtBackground = nullptr;
	stb::Texture* m_txtConfirmNormal = nullptr;
	stb::Texture* m_txtTradeNormal = nullptr;
	stb::Texture* m_txtTradeChecked = nullptr;
	stb::Texture* m_txtLayerConfirmMe = nullptr;

	int m_posX = 230;
	int m_posY = 100;

	//슬롯 레이아웃 (InventoryUI와 동일한 방식)
	static constexpr int SLOT_COLS = 4;
	static constexpr int SLOT_ROWS = 4;
	static constexpr int SLOT_W = 32;
	static constexpr int SLOT_H = 32;
	static constexpr int SLOT_GAP_X = 2;
	static constexpr int SLOT_GAP_Y = 2;

	static constexpr int BUTTON_CONFIRM_X = 10;
	static constexpr int BUTTON_CONFIRM_Y = 580;
	static constexpr int BUTTON_TRADE_X = 225;
	static constexpr int BUTTON_TRADE_Y = 580;

	static constexpr int LAYER_CONFIRM_ME_X = 227;
	static constexpr int LAYER_CONFIRM_ME_Y = 43;
	static constexpr int LAYER_CONFIRM_TARGET_X = 20;
	static constexpr int LAYER_CONFIRM_TARGET_Y = 43;
	
	std::wstring m_myName;
	std::string m_myId;

	std::wstring m_targetName;
	std::string m_targetId;
};