#include "stbAnimator.h"
#include "stbTransform.h"
#include "stbResourceManager.h"
#include <filesystem>

#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()


namespace stb
{
	
	Animator::Animator()
		:Component(eComponentType::Animator)
		, mAnimations({})
		, mActiveAnimation(nullptr)
		, mbLoop(false)
	{

	}

	Animator::~Animator()
	{

	}

	void Animator::Initialize()
	{
	}

	void Animator::Update()
	{
		if (mActiveAnimation)
		{
			mActiveAnimation->Update();

			Events* events = FindEvent(mActiveAnimation->GetName());

			if (mActiveAnimation->IsComplete())
			{
				if (events)
				{
					events->completeEvent.mEvent;
				}
				if(mbLoop)
				{
					mActiveAnimation->Reset();
				}
			}
		}
	}

	void Animator::LateUpdate()
	{
	}

	void Animator::Render(HDC hdc)
	{
		if (mActiveAnimation)
		{
			//  현재 활성화된 애니메이션 Render
			mActiveAnimation->Render(hdc);
		}
	}

	void Animator::CreateAnimation(const std::wstring& name
		, Texture* spriteTexture
		, Vector2 leftTop
		, Vector2 size
		, Vector2 offset
		, UINT spriteLength
		, float duration)
	{
		Animation* animation = nullptr;

		animation = FindAnimation(name);
		if (animation)
		{
			return;
		}

		animation = new Animation();
		animation->SetName(name);
		animation->CreateAnimation(name, spriteTexture, leftTop, size, offset, spriteLength, duration);

		animation->SetAnimator(this);

		Events* events = new Events();

		mAnimations.insert(make_pair(name, animation));
		mEvents.insert(make_pair(name, events));

	}

	// 현재 이미지에서 y 값은 고려하지 않고 x값만 고려해서 애니메이션을 만든다.
	void Animator::CreateAnimationByFolder(const std::wstring& name
		, const std::wstring& path
		, Vector2 offset, float duration)
	{
		Animation* animation = nullptr;
		animation = FindAnimation(name);

		if (animation != nullptr)
		{
			return;
		}

		int fileCount = 0;
		std::filesystem::path fs(path);
		std::vector<Texture*> images = {};

		for (auto& p : std::filesystem::recursive_directory_iterator(fs))
		{
			std::wstring fileName = p.path().filename();
			std::wstring filePath = p.path();

			Texture* texture = M_REMANAGER->Load<Texture>(fileName, filePath);
			images.push_back(texture);
			fileCount++;
		}

		UINT sheetWidth = images[0]->GetWidth() * fileCount;
		UINT sheetHeight = images[0]->GetHeight();
		Texture* spriteSheet = Texture::Create(name, sheetWidth, sheetHeight);

		UINT imageWidth = images[0]->GetWidth();
		UINT imageHeight = images[0]->GetHeight();

		for (size_t i = 0; i < images.size(); i++)
		{
			BitBlt(spriteSheet->GetHdc(), i * imageWidth, 0
				, imageWidth, imageHeight
				, images[i]->GetHdc(), 0, 0, SRCCOPY);
		}

		CreateAnimation(name, spriteSheet
			, Vector2(0.0f, 0.0f), Vector2(imageWidth, imageHeight)
			, offset, fileCount, duration);

	}


	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		auto iter = mAnimations.find(name);
		if (iter == mAnimations.end())
		{
			return nullptr;
		}
		return iter->second;
	}


	void Animator::PlayAnimation(const std::wstring& name, bool loop)
	{
		Animation* animation = FindAnimation(name);

		if (animation == nullptr)
		{
			return;
		}

		if (mActiveAnimation)
		{
			Events* currEvent = FindEvent(mActiveAnimation->GetName());

			if (currEvent)
			{
				currEvent->endEvent.mEvent;
			}
		}

		Events* nextEvent = FindEvent(name);
		
		if (nextEvent)
		{
			nextEvent->startEvent();
		}
		mActiveAnimation = animation;
		mActiveAnimation->Reset();
		mbLoop = loop;

	}

	Animator::Events* Animator::FindEvent(const std::wstring& name)
	{
		auto iter = mEvents.find(name);

		if (iter == mEvents.end())
		{
			return nullptr;
		}
		return iter->second;
	}

	std::function<void()>& Animator::GetStartEvent(const std::wstring& name)
	{
		Events* event = FindEvent(name);

		return event->startEvent.mEvent;
	}

	std::function<void()>& Animator::GetCompleteEvent(const std::wstring& name)
	{
		Events* event = FindEvent(name);
		return event->completeEvent.mEvent;
	}

	std::function<void()>& Animator::GetEndEvent(const std::wstring& name)
	{
		Events* event = FindEvent(name);
		return event->endEvent.mEvent;
	}
}