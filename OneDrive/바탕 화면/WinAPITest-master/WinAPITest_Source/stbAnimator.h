#pragma once
#include "stbComponent.h"
#include "stbTexture.h"
#include "stbAnimation.h"
namespace stb
{
	class Animator : public Component 
	{
	public:
		struct Event
		{
			void operator=(std::function<void()>&& func)
			{
				mEvent = move(func);
			}

			void operator()()
			{
				if (mEvent)
				{
					mEvent();
				}
			}

			std::function<void()> mEvent;
		};

		
		// ev2 = ev1;
		// ev2 = std::function<void()>(Add);

		struct Events
		{
			Event startEvent;
			Event completeEvent;
			Event endEvent;
		};

		Events* FindEvent(const std::wstring& name);
		std::function<void()>& GetStartEvent(const std::wstring& name);
		std::function<void()>& GetCompleteEvent(const std::wstring& name);
		std::function<void()>& GetEndEvent(const std::wstring& name);

		Animator();
		~Animator();

		void Initialize() override;
		void Update() override; 
		void LateUpdate() override;
		void Render(HDC hdc) override;
		void Render(stbD2DRenderer& renderer) override;

		void CreateAnimation(const std::wstring& name
			, Texture* spriteTexture
			, Vector2 leftTop
			, Vector2 size
			, Vector2 offset
			, UINT spriteLength
			, float duration);

		void CreateAnimationByFolder(const std::wstring& name
			, const std::wstring& path
			, Vector2 offset, float duration);

		Animation* FindAnimation(const std::wstring& name);
		void PlayAnimation(const std::wstring& name, bool loop);

	private:
		std::map<std::wstring, Animation*> mAnimations;
		Animation* mActiveAnimation;
		bool mbLoop;

		std::map<std::wstring, Events*> mEvents;


	};
}
