#include "stbChatScene.h"

#include "stbNetworkDebug.h"

#include "stbApplication.h"
#include "stbChatNetworkManager.h"
#include "ChatPacketHandler.h"
#include "stbInput.h"

#include "StringConvert.h"

//#include "PacketManager.h"
//#include "stbLogger.h"
//#include "PlayerManager.h"
//#include "stbTime.h"
//#include "stbGameObject.h"
//#include "stbPlayer.h"
//#include "stbObject.h"
//#include "stbTransform.h"
//#include "stbTexture.h"
//#include "stbResourceManager.h"
//#include "stbSpriteRenderer.h"
//#include "stbPlayerScript.h"
//#include "stbCamera.h"
//#include "stbAnimator.h"
//#include "stbRender.h"
//#include "UIManager.h"

#define M_CHAT_NETMANAGER stb::SingletonBase<ChatNetworkManager>::getInstance()
#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()

//#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
//#define M_PKMANAGER stb::SingletonBase<PacketManager>::getInstance()
//#define M_PLMANAGER stb::SingletonBase<PlayerManager>::getInstance()
//#define M_TIME	stb::SingletonBase<stb::Time>::getInstance()
//#define M_UIMANAGER stb::SingletonBase<UIManager>::getInstance()

namespace stb
{
	ChatScene::ChatScene() // :mPlayer(nullptr)
	{
	}

	ChatScene::~ChatScene()
	{
	}

	void ChatScene::Initialize()
	{
		Scene::Initialize();

		// 채팅용 네트워크 매니저 초기화
		HWND hWnd = stb::Application::getInstance()->GetHWND();

		// 핸들러 등록
		ChatNetworkManager::getInstance()->RegisterHandler(PKT_CHAT_INIT,
			[](const ParsedPacket& pkt) { ChatPacketHandler::HandleChatInit(pkt); });
		ChatNetworkManager::getInstance()->RegisterHandler(PKT_CHAT,
			[](const ParsedPacket& pkt) { ChatPacketHandler::HandleChat(pkt); });

		// 채팅 서버 접속 (PlayScene의 채널 서버와는 별개)
		ChatNetworkManager::getInstance()->Connect(
			NetworkConfig::SERVER_IP,
			g_ChatPort,
			hWnd);

	}

	void ChatScene::Update()
	{
		Scene::Update();
		//M_UIMANAGER->Update();

		// 입력 처리 (예: Enter → 입력 포커스 토글)
		if (M_INPUT->GetKeyDown(eKeyCode::Enter)) 
			ToggleInputFocus();

		if (mInputActive)
		{
			// 텍스트 입력 처리 (간단히는 Input에서 키 이벤트로 mInputBuffer 갱신)
			if (M_INPUT->GetKeyDown(eKeyCode::Return) && !mInputBuffer.empty())
			{
				// UTF-16 → UTF-8 변환 후 송신
				std::string utf8 = Convert::WstrToUtf8(mInputBuffer);
				ChatPacketHandler::SendChat(utf8);
				mInputBuffer.clear();
			}
		}
	}

	void ChatScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void ChatScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}

	void ChatScene::Render(stbD2DRenderer& renderer)
	{
		Scene::Render(renderer);
		//M_UIMANAGER->Render(renderer);

		// 화면 하단에 반투명 박스 + 텍스트 라인들 + 입력박스 그리기
		
		
		//DrawChatBackdrop(renderer);
		//DrawChatLog(renderer, mLogLines);
		//if (mInputActive) DrawChatInput(renderer, mInputBuffer);
		
	}

	void ChatScene::OnExit()
	{
		Scene::OnExit();
	}

	void ChatScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void ChatScene::ToggleInputFocus()
	{
	}

}
