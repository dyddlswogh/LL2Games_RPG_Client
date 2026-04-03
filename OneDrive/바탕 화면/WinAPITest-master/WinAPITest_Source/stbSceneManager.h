#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"
#include "stbScene.h"

namespace stb
{
	class SceneManager : public SingletonBase<SceneManager>
	{
	public:
		SceneManager();
		~SceneManager();

		// scene 만들기
		template <typename T>
		Scene* CreateScene(const std::wstring& name)
		{
			T* scene = new T();
			scene->SetName(name);
			mActiveScene = scene;
			scene->Initialize();

			mScenes.insert(std::make_pair(name, scene));
			return scene;
		}


		void Initialize();
		void Update();
		void Render(HDC hdc);

		Scene* LoadScene(const std::wstring& name);
		Scene* GetActiveScene() { return mActiveScene; }
		void SetActiveScene(Scene* scene) { mActiveScene = scene; }
		Scene* GetDontDestroyOnLoad() { return mDontDestroyOnLoad; }

		std::vector<GameObject*> GetGameObjects(eLayerType layer);
		
	private:
		// 씬들을 map을 통해 가지고 있고 현재 실행중인 씬만 화면에 출력
		std::map<std::wstring, Scene*> mScenes;
		Scene* mActiveScene;
		Scene* mDontDestroyOnLoad;

	};
}

