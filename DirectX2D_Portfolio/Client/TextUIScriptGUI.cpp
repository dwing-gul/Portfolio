#include "pch.h"
#include "TextUIScriptGUI.h"
#include <Engine/KeyManager.h>
#include <Engine/FontManager.h>
#include <Script/TextUIScript.h>

const char* TargetTypeString[(UINT)TEXT_TARGET_TYPE::END] =
{
	"General",
	"Player",
	"Monster",
	"Item"
};

const char* TextUsageString[(UINT)TEXT_USAGE::END] =
{
	"Name",
	"Job",
	"HP",
	"Stress",
	"Accuracy",
	"Critical",
	"Damage",
	"Dodge",
	"Protection",
	"Speed",
	"Resistance_Stun",
	"Resistance_Blight",
	"Resistance_Bleed",
	"Resistance_Debuff",
	"Resistance_Move",
	"Item_Count",
	"General",
};

TextUIScriptGUI::TextUIScriptGUI() :
	ScriptGUI(SCRIPT_TYPE::TEXTUISCRIPT)
{
}

TextUIScriptGUI::~TextUIScriptGUI()
{
}

void TextUIScriptGUI::update()
{
	if (Start())
	{
		TEXT_TARGET_TYPE targetType = ((TextUIScript*)GetScript())->GetTargetType();
		TEXT_USAGE usage = ((TextUIScript*)GetScript())->GetUsage();
        string contents = WStringToString(((TextUIScript*)GetScript())->GetText());
        Vector2 textPos = ((TextUIScript*)GetScript())->GetTextPos();
        float fontSize = ((TextUIScript*)GetScript())->GetFontSize();
		
		int red = ((TextUIScript*)GetScript())->GetFontColor_Red();
		int green = ((TextUIScript*)GetScript())->GetFontColor_Green();
		int blue = ((TextUIScript*)GetScript())->GetFontColor_Blue();
		int alpha = ((TextUIScript*)GetScript())->GetFontColor_Alpha();

		bool show = ((TextUIScript*)GetScript())->IsShow();

		ImGui::Text("Target Type");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		int targetTypeidx = (int)targetType; // Here we store our selection data as an index.
		const char* combo_preview_value = TargetTypeString[targetTypeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##TargetType", combo_preview_value))
		{
			for (UINT i = 0; i < (UINT)TEXT_TARGET_TYPE::END; i++)
			{
				const bool is_selected = (targetTypeidx == i);
				if (ImGui::Selectable(TargetTypeString[i], is_selected))
				{
					targetTypeidx = i;
					targetType = (TEXT_TARGET_TYPE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		((TextUIScript*)GetScript())->SetTargetType(targetType);

		ImGui::Text("Text Usage");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		int textUsageidx = (int)usage; // Here we store our selection data as an index.
		const char* combo_preview_value2 = TextUsageString[textUsageidx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##TextUsage", combo_preview_value2))
		{
			for (UINT i = 0; i < (UINT)TEXT_USAGE::END; i++)
			{
				const bool is_selected = (textUsageidx == i);
				if (ImGui::Selectable(TextUsageString[i], is_selected))
				{
					textUsageidx = i;
					usage = (TEXT_USAGE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		((TextUIScript*)GetScript())->SetUsage(usage);

        char textContents[255] = {};
        strcpy_s(textContents, 255, contents.c_str());
        ImGui::InputText("##TextUI", textContents, 255);

		if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
		{
			wstring text = StringToWString(textContents);
			((TextUIScript*)GetScript())->SetText(text);
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

		((TextUIScript*)GetScript())->SetTextPos(textPos);
		((TextUIScript*)GetScript())->SetFontSize(fontSize);
		((TextUIScript*)GetScript())->SetFontColor_Red(red);
		((TextUIScript*)GetScript())->SetFontColor_Green(green);
		((TextUIScript*)GetScript())->SetFontColor_Blue(blue);
		((TextUIScript*)GetScript())->SetFontColor_Alpha(alpha);
		((TextUIScript*)GetScript())->SetShow(show);

		((TextUIScript*)GetScript())->SetFontColor(FONT_RGBA(red, green, blue, alpha));

		End();
	}
}
