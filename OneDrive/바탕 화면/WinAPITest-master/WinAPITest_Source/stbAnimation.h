#pragma once
#include "stbResource.h"
#include "stbTexture.h"

class stbD2DRenderer;

namespace stb
{
	using namespace math;

	class Animation : public Resource
	{
	public:
		struct Sprite
		{
			Vector2 leftTop;
			Vector2 size;
			Vector2 offset;
			float duration;
			Texture* texture = nullptr;


			Sprite()
				: leftTop(Vector2::Zero)
				, size(Vector2::Zero)
				, offset(Vector2::Zero)
				, duration(0.0f)
			{

			}
		};

		Animation();
		~Animation();


		void Update();
		void Render(HDC hdc);
		void Render(stbD2DRenderer& renderer);

		void CreateAnimation(const std::wstring& name
			, Texture* spriteTexture
			, Vector2 leftTop
			, Vector2 size
			, Vector2 offset
			, UINT spriteLength
			, float duration
		);

		void CreateFrameAnimation(const std::wstring& name
			, const std::vector<Texture*>& frames
			, Vector2 offset
			, float duration
		);

		void Reset();
		HRESULT Load(const std::wstring& path);

		bool IsComplete() { return mbComplete; }
		void SetAnimator(class Animator* animator) { mAnimator = animator; }

	private:
		class Animator* mAnimator;
		Texture* mTexture;

		std::vector<Sprite> mAnimationSheet;
		int mIndex;
		float mTime;
		bool mbComplete;

	};

}

