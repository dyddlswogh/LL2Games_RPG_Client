#pragma once
#include "stbResource.h"
#include "stbSingletonBase.h"
#include "stbTexture.h"
namespace stb
{
	class ResourceManager : public SingletonBase<ResourceManager>
	{
	public:

		ResourceManager();
		~ResourceManager();
		
		template <typename T>
		T* Find(const std::wstring& name)
		{
			auto iter = mResources.find(name);

			if (iter == mResources.end())
			{
				return nullptr;
			}

			return dynamic_cast<T*>(iter->second);
		}

		template<typename T>
		T* Load(const std::wstring& name, const std::wstring& path)
		{
			T* resource = Find<T>(name);

			if (resource != nullptr)
			{
				return resource;
			}

			resource = new T();
			if (FAILED(resource->Load(path)))
			{
				assert(false);
			}

			resource->SetName(name);
			resource->SetPath(path);
			mResources.insert(std::make_pair(name, resource));

			return resource;
		}


		
		void Insert(const std::wstring& name, Resource* resource)
		{
			if (name == L"")
				return;
			if (resource == nullptr)
				return;
			mResources.insert(make_pair(name, resource));
		}

		void LoadAllD2D(stbD2DRenderer& renderer);
		void LoadMonsterTextures();
		void LoadPlayerTextures();
		void LoadItemTextures();
		void LoadHealthBarTextures();
		void LoadExpTextures();
		void LoadLevelTextures();

	private:
		std::map<std::wstring, Resource*> mResources;
	};

}

