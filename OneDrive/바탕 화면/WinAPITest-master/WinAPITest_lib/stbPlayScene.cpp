#include "stbPlayScene.h"
#include "stbGameObject.h"
#include "stbPlayer.h"
#include "stbObject.h"
#include "stbTransform.h"
#include "stbTexture.h"
#include "stbResourceManager.h"
#include "stbSpriteRenderer.h"
#include "stbPlayerScript.h"
#include "stbCamera.h"
#include "stbAnimator.h"
#include "stbRender.h"
#include "PacketManager.h"
#include "stbNetworkDebug.h"
#include "stbLogger.h"
#include "PlayerManager.h"
#include "UIManager.h"

#include "stbApplication.h"

#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
#define M_PKMANAGER stb::SingletonBase<PacketManager>::getInstance()
#define M_PLMANAGER stb::SingletonBase<PlayerManager>::getInstance()
#define M_UIMANAGER stb::SingletonBase<UIManager>::getInstance()

namespace stb
{
	PlayScene::PlayScene()
		:mPlayer(nullptr)
	{
	}

	PlayScene::~PlayScene()
	{
	}

	void PlayScene::Initialize()
	{
		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::Particle, Vector2(0.0f, 0.0f));
		Camera* cameraComp = camera->AddComponent<Camera>();
		render::mainCamera = cameraComp;

		mPlayer = object::Instantiate<Player>(enums::eLayerType::Player);

		Transform* tr = mPlayer->AddComponent<Transform>();
		int charId = atoi(stb::NetworkConfig::GetCharacterId());
		//if (charId < 1 || charId > 4) charId = 1;
		//float startX = 500.0f - (110.0f * 4);	
		//float posX = startX + (charId - 1) * 110.0f;
		tr->SetPosition(Vector2(300.0f, 300.0f));

		PlayerScript* playerScript = mPlayer->AddComponent<PlayerScript>();
		M_PLMANAGER->SetLocalPlayer(mPlayer);

		M_UIMANAGER->Init();
		Texture* spartaTex = M_REMANAGER->Find<Texture>(L"Sparta");
		Animator* spartaAnim = mPlayer->AddComponent<Animator>();
		if (spartaTex != nullptr)
		{
			spartaAnim->CreateAnimation(L"Run", spartaTex, Vector2(990.0f, 192.0f), Vector2(110.0f, 96.0f), Vector2::Zero, 4, 0.15f);
			spartaAnim->PlayAnimation(L"Run", true);
		}

		playerScript->SetFollowers(nullptr, nullptr);

		Scene::Initialize();

		stb::Logger::Init();
		LOG("========== Client Start (ID: " << stb::NetworkConfig::GetCharacterId() << ") ==========\n");

		HWND hWnd = stb::Application::getInstance()->GetHWND();
		M_PKMANAGER->RegisterAllHandlers();
		LOG("========== Network Connect Start ==========\n");
		stb::InitializeNetworkDebug(hWnd);
	}

	void PlayScene::Update()
	{
		Scene::Update();
	}

	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}

	void PlayScene::Render(stbD2DRenderer& renderer)
	{
		Scene::Render(renderer);
		M_UIMANAGER->Render(renderer);
	}

	void PlayScene::OnExit()
	{
		Scene::OnExit();
	}

	void PlayScene::OnEnter()
	{
		Scene::OnEnter();
	}
}
