#include "TradeUI.h"
#include "stbResourceManager.h"
#include "stbApplication.h"
#include "stbInput.h"
#include "InventoryManager.h"
#include "TradePacketHandler.h"

#define M_APP stb::SingletonBase<stb::Application>::getInstance()
#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()
#define M_INVENTORYMANAGER stb::SingletonBase<InventoryManager>::getInstance()
#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()


void TradeUI::Init()
{
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