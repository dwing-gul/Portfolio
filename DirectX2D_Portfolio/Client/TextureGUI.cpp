#include "pch.h"
#include "TextureGUI.h"
#include <Engine/Texture.h>

TextureGUI::TextureGUI() :
	ResourceGUI(RESOURCE_TYPE::TEXTURE)
{
}

TextureGUI::~TextureGUI()
{
}

void TextureGUI::update()
{
	if (Start())
	{
		Texture* tex = (Texture*)GetTarget();

		ImVec2 uvMin = ImVec2(0.f, 0.f);
		ImVec2 uvMax = ImVec2(1.f, 1.f);
		ImVec4 tintColor = ImVec4(1.f, 1.f, 1.f, 1.f);
		ImVec4 borderColor = ImVec4(1.f, 1.f, 1.f, 0.5f);
		ImGui::Image((ImTextureID)tex->GetSRV().Get(), ImVec2(150.f, 150.f), uvMin, uvMax, tintColor, borderColor);

		End();
	}
}
