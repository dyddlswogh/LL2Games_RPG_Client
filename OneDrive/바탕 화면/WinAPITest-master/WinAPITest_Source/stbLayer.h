#pragma once
#include "stbEntity.h"
#include "stbGameObject.h"


namespace stb 
{
	class Layer : public Entity
	{
	public:
		Layer();
		~Layer();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render(HDC hdc);

		void AddGameObject(GameObject* gameObject);
		const std::vector<GameObject*> GetGameObjects() { return mGameObjects; }
	private:
		std::vector<GameObject*> mGameObjects;

	};

}

