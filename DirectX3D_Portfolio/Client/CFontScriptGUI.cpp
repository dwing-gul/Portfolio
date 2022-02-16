#include "pch.h"
#include "CFontScriptGUI.h"
#include <Engine/CFontManager.h>
#include <Script/CFontUIScript.h>

const char* FontTypeString[(UINT)FONT_TYPE::END] =
{
	"General",
	"Day",
	"Attack Power",
	"Defense",
	"Extra Attack",
	"Damage Reduction",
	"Skill DMG Inc",
	"Skill DMG Reduction",
	"Move Speed",
	"Attack Speed",
	"HP",
	"SP",
	"User Num",
	"Time",
	"Level",
	"Tracking Level"
};

CFontScriptGUI::CFontScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::FONTUISCRIPT)
{
}

CFontScriptGUI::~CFontScriptGUI()
{
}

void CFontScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		CFontUIScript* fontScript = (CFontUIScript*)GetTarget()->GetScriptComponent((UINT)SCRIPT_TYPE::FONTUISCRIPT);

		string contents = WStringToString(fontScript->GetText());
		FONT_TYPE fontType = fontScript->GetFontType();
		Vector2 textPos = fontScript->GetTextPos();
		float fontSize = fontScript->GetFontSize();

		int red = fontScript->GetFontColor_Red();
		int green = fontScript->GetFontColor_Green();
		int blue = fontScript->GetFontColor_Blue();
		int alpha = fontScript->GetFontColor_Alpha();

		bool show = fontScript->IsShow();

		char textContents[255] = {};
		strcpy_s(textContents, 255, contents.c_str());
		ImGui::InputText("##Text_FontUI", textContents, 255);

		if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
		{
			wstring text = StringToWString(textContents);
			fontScript->SetText(text);
		}

		ImGui::Text("Font Type");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		int fontTypeidx = (int)fontType; // Here we store our selection data as an index.
		const char* combo_preview_value_font = FontTypeString[fontTypeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##FontType", combo_preview_value_font))
		{
			for (UINT i = 0; i < (UINT)FONT_TYPE::END; i++)
			{
				const bool is_selected = (fontTypeidx == i);
				if (ImGui::Selectable(FontTypeString[i], is_selected))
				{
					fontTypeidx = i;
					fontType = (FONT_TYPE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("TextPosition");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat2("##TextPosition", textPos);

		ImGui::Text("FontSize");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##FontSize", &fontSize);

		ImGui::Text("FontColor Red");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##FontColor_Red", &red);

		ImGui::Text("FontColor Green");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##FontColor_Green", &green);

		ImGui::Text("FontColor Blue");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##FontColor_Blue", &blue);

		ImGui::Text("FontColor Alpha");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##FontColor_Alpha", &alpha);

		ImGui::Text("Text Show");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::Checkbox("##TextShow", &show);

		fontScript->SetTextPos(textPos);
		fontScript->SetFontType(fontType);
		fontScript->SetFontSize(fontSize);
		fontScript->SetFontColor_Red(red);
		fontScript->SetFontColor_Green(green);
		fontScript->SetFontColor_Blue(blue);
		fontScript->SetFontColor_Alpha(alpha);
		fontScript->SetShow(show);
		fontScript->SetFontColor(FONT_RGBA(red, green, blue, alpha));

		End();
	}
}
