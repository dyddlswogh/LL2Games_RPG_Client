#include "stbD2DRenderer.h"
#include "stbTexture.h"
#include "stbResourceManager.h"
#include <nlohmann/json.hpp>

stb::ResourceManager::ResourceManager()
{
}

stb::ResourceManager::~ResourceManager()
{
}

void stb::ResourceManager::LoadAllD2D(stbD2DRenderer& renderer)
{
    for (auto& pair : mResources)
    {
        stb::Texture* tex = dynamic_cast<stb::Texture*>(pair.second);
        if (tex) tex->LoadD2D(renderer);
    }
}

void stb::ResourceManager::LoadMonsterTextures()
{
	std::filesystem::path root = L"Resources\\Monster";

	for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}
			

		if (entry.path().extension() != ".png")
		{
			continue;
		}
			

		std::filesystem::path relative = std::filesystem::relative(entry.path(), L"Resources");

		std::wstring key = relative.replace_extension(L"").generic_wstring();

		OutputDebugStringW(key.c_str());
		OutputDebugStringA("\n");
		Load<Texture>(key, entry.path().wstring());
	}
}
