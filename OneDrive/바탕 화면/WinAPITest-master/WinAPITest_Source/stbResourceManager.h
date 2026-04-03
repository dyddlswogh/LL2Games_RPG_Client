#pragma once
#include "stbResource.h"
#include "stbSingletonBase.h"
namespace stb
{
	class ResourceManager : public SingletonBase<ResourceManager>
	{
	public:

		ResourceManager();
		~ResourceManager();
		// 찾기 / 로드 / 추가 
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
			// 정적 멤버 함수인 경우 어떤 클래스의 함수인지 모호함이 발생할 수 있기 떄문에 적어주는게 좋다.
			// 하지만 나는 싱글톤으로 만들어서 사용할 것이기 때문에 굳이 할 필요는 없다.
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
			// 리소스 이름이 안적혀 있는 경우와 리소스가 nullptr인 경우 안넣음
			if (name == L"")
			{
				return;
			}

			if (resource == nullptr)
			{
				return;
			}

			mResources.insert(make_pair(name, resource));
		}



	private:
		std::map<std::wstring, Resource*> mResources;
	};

}

