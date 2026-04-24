#pragma once
#include "CommonInclude.h"
#include "stbEntity.h"
#include "stbLayer.h"

class stbD2DRenderer;


namespace stb
{
	class Scene : public Entity
	{
	public:
		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);
		virtual void Render(stbD2DRenderer& renderer);

		virtual void OnExit();
		virtual void OnEnter();

		void AddGameObject(GameObject* gameObject, const enums::eLayerType layertype);
		Layer* GetLayer(enums::eLayerType type) { return mLayers[(UINT)type]; }

	private:
		void CreateLayers();
	private:
		std::vector<Layer*> mLayers;
	};

}


