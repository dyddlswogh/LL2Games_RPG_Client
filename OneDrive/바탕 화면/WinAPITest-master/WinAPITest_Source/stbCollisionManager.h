#pragma once
#include "stbSingletonBase.h"
#include "CommonInclude.h"
#include "stbCollider.h"

namespace stb
{
	union ColliderID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};

		UINT64 id;
	};

	class CollisionManager : public SingletonBase<CollisionManager>
	{
	public:
		CollisionManager();
		~CollisionManager();

		void Initailzie();
		void Update();
		void LateUpdate();
		void Render(HDC hdc);
		void Clear();

		void CollisionLayerCheck(eLayerType left, eLayerType right, bool enable);
		void LayerCollosion(class Scene* scene, eLayerType left, eLayerType right);
		void ColliderCollision(Collider* left, Collider* right);
		bool Intersect(Collider* left, Collider* right);


	private:
		std::bitset<(UINT)eLayerType::Max> mCollisionLayerMatrix[(UINT)eLayerType::Max];
		std::unordered_map <UINT64, bool> mCollisionMap;
	};


}

