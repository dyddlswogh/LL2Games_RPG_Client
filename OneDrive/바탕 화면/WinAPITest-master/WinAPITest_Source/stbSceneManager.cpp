#include "stbSceneManager.h"
#include "stbDontDestroyOnLoad.h"

namespace stb
{
	SceneManager::SceneManager()
		: mScenes({})
		, mActiveScene(nullptr)
		, mDontDestroyOnLoad(nullptr)
	{

	}

	SceneManager::~SceneManager()
	{

	}

	Scene* SceneManager::LoadScene(const std::wstring& name)
	{
		if (mActiveScene)
		{
			mActiveScene->OnExit();
		}
		
		std::map<std::wstring, Scene*>::iterator iter = mScenes.find(name);

		if (iter == mScenes.end())
		{
			return nullptr;
		}

		mActiveScene = iter->second;
		mActiveScene->OnEnter();

		return iter->second;
		
	}


	void SceneManager::Initialize()
	{
		mDontDestroyOnLoad = CreateScene<DontDestroyOnLoad>(L"DontDestroyOnLoad");
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
		mDontDestroyOnLoad->Update();
	}

	void SceneManager::Render(HDC hdc)
	{
		mActiveScene->Render(hdc);
		mDontDestroyOnLoad->Render(hdc);
	}

	std::vector<GameObject*> SceneManager::GetGameObjects(eLayerType layer)
	{
		std::vector<GameObject*> gameObjects =
			mActiveScene->GetLayer(layer)->GetGameObjects();

		std::vector<GameObject*> donDestroyOnLoad =
			mDontDestroyOnLoad->GetLayer(layer)->GetGameObjects();


		gameObjects.insert(gameObjects.end(), donDestroyOnLoad.begin(), donDestroyOnLoad.end());

		return gameObjects;
	}

}