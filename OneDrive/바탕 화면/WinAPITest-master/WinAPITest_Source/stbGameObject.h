
#pragma once
#include "CommonInclude.h"
#include "stbComponent.h"


namespace stb
{
	class GameObject : public Entity
	{
	public:
		GameObject();
		~GameObject();

		// 템플릿을 활용해 여러 컴포넌트들을 게임 오브젝트에 추가 하는 함수
		template <typename T>
		T* AddComponent()
		{
			T* component = new T();
			component->SetOwner(this);

			mComponents[(UINT)component->GetType()] = component;

			return component;
		}

		template<typename T>
		T* GetComponent()
		{
			T* component = nullptr;
			for (Component* comp : mComponents)
			{
				component = dynamic_cast<T*>(comp);
				if (component)
					break;
			}

			return component;
		}

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);

		void SetLayerType(eLayerType type) { mLayerType = type; }
		eLayerType GetLayerType() {return mLayerType; }
	private:

		// 게임 오브젝트가 필요한 기능들인 컴포넌트들을 들고 있기 위해 Vector로 저장
		std::vector<Component*> mComponents;
		eLayerType mLayerType;

	};

}

