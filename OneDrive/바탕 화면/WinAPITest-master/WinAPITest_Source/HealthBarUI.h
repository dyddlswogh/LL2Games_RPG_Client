#pragma once
#include "UI.h"
#include "UILayout.h"
#include "stbTexture.h"

class HealthBarUI : public UI
{
public:
	void Init() override;
	void Update() override;
	void Render(stbD2DRenderer& renderer) override;

	void RenderBackGround(stbD2DRenderer& renderer);
	void RenderHpLayer(stbD2DRenderer& renderer);
	void RenderMpLayer(stbD2DRenderer& renderer);

	void RenderHpText(stbD2DRenderer& renderer);
	void RenderMpText(stbD2DRenderer& renderer);

private:	
	stb::Texture* m_background = nullptr;
	stb::Texture* m_hpLayer = nullptr;
	stb::Texture* m_mpLayer = nullptr;
	stb::Texture* m_guage = nullptr;

	UIRect m_UIRect;
private:

	
};

