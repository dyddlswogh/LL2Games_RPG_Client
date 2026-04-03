#include "stbD2DRenderer.h"
#include "stbTexture.h"
#include "stbResourceManager.h"

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
