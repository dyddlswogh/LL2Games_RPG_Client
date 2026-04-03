#pragma once
#include "stbResource.h"


namespace stb
{
	class Texture : public Resource
	{
	public:
		enum class eTextureType
		{
			Bmp,
			Png,
			None,
		};

		static Texture* Create(const std::wstring& name, UINT width, UINT height);

		Texture();
		~Texture();
		
		virtual HRESULT Load(const std::wstring& path) override;

		void SetWidth(UINT width) { mWidth = width;}
		UINT GetWidth() { return mWidth; }
		void SetHegith(UINT height) { mHeight = height; }
		UINT GetHeight() { return mHeight; }
		eTextureType GetTextureType() { return mType; }
		Gdiplus::Image* GetImage() { return mImage; }
		bool IsAlpha() { return mAlpha; }

		HDC GetHdc() { return mHdc; }

	private:
		bool mAlpha;
		eTextureType mType;

		HBITMAP mBitmap;
		HDC mHdc;

		UINT mWidth;
		UINT mHeight;

		Gdiplus::Image* mImage;

	};

}

