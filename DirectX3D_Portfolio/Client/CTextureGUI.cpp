#include "pch.h"
#include "CTextureGUI.h"
#include <Engine/CTexture.h>

CTextureGUI::CTextureGUI() :
	CResourceGUI(RESOURCE_TYPE::TEXTURE)
{
}

CTextureGUI::~CTextureGUI()
{
}

void CTextureGUI::update()
{
	if (Start())
	{
		CTexture* tex = (CTexture*)GetTarget();

		ImVec2 uvMin = ImVec2(0.f, 0.f);
		ImVec2 uvMax = ImVec2(1.f, 1.f);
		ImVec4 tintColor = ImVec4(1.f, 1.f, 1.f, 1.f);
		ImVec4 borderColor = ImVec4(1.f, 1.f, 1.f, 0.5f);
		ImGui::Image((ImTextureID)tex->GetSRV().Get(), ImVec2(150.f, 150.f), uvMin, uvMax, tintColor, borderColor);

		End();
	}
}
