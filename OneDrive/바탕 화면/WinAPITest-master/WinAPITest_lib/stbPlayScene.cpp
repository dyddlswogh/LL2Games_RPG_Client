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
#include "MonsterManager.h"
#include "stbTime.h"

#include "stbApplication.h"

#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
#define M_PKMANAGER stb::SingletonBase<PacketManager>::getInstance()
#define M_PLMANAGER stb::SingletonBase<PlayerManager>::getInstance()
#define M_UIMANAGER stb::SingletonBase<UIManager>::getInstance()
#define M_MONSTERAMANGER stb::SingletonBase<MonsterManager>::getInstance()
#define M_TIME	stb::SingletonBase<stb::Time>::getInstance()

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

	
		mPlayer = M_PLMANAGER->CreateLocalPlayer(enums::eLayerType::Player, Vector2(300.0f, 300.0f));
		int charId = atoi(stb::NetworkConfig::GetCharacterId());
		
		M_UIMANAGER->Init();
		Scene::Initialize();

		stb::Logger::Init();
		LOG("========== Client Start (ID: " << stb::NetworkConfig::GetCharacterId() << ") ==========\n");

		HWND hWnd = stb::Application::getInstance()->GetHWND();
		M_PKMANAGER->RegisterAllHandlers();
		LOG("========== Network Connect Start ==========\n");
		stb::InitializeNetworkDebug(hWnd);


		M_MONSTERAMANGER->Init();
	}

	void PlayScene::Update()
	{
		Scene::Update();
		M_UIMANAGER->Update();
		M_MONSTERAMANGER->Update(M_TIME->GetDeltaTime());
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
		M_MONSTERAMANGER->Render(renderer);
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
