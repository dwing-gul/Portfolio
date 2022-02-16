#include "pch.h"
#include "CMaterialGUI.h"
#include <Engine/CMaterial.h>
#include <Engine/CResourceManager.h>
#include "CListGUI.h"
#include "CParamGUI.h"
#include "CImGuiManager.h"

CMaterialGUI::CMaterialGUI() :
	CResourceGUI(RESOURCE_TYPE::MATERIAL),
	m_selectedTextureParam(SHADER_PARAM::TEXTURE_END)
{
}

CMaterialGUI::~CMaterialGUI()
{
}

void CMaterialGUI::update()
{
	if (Start())
	{
		CMaterial* material = (CMaterial*)GetTarget();
		CShader* shader = material->GetShader().Get();

		char shaderName[255] = {};
		GetResourceName(shader, shaderName);

		ImGui::Text("Shader");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Shader", shaderName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Shader"))
		{
			CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Shader");
			vector<wstring> vectorName;
			CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::GRAPHICS, vectorName);
			listGUI->SetList(vectorName);
			listGUI->SetDoubleClickedCallback(this, (GUI_CALLBACK)&CMaterialGUI::SetShader);
			listGUI->Activate();
		}

		if (nullptr != shader)
		{
			const vector<tShaderParam>& vectorParam = shader->GetShaderParam();

			for (size_t i = 0; i < vectorParam.size(); ++i)
			{
				string name = string(vectorParam[i].name.begin(), vectorParam[i].name.end());

				char buffer[255] = {};
				sprintf_s(buffer, "##%lld", (DWORD_PTR)shader);

				name += buffer;

				switch (vectorParam[i].type)
				{
				case SHADER_PARAM::INT_0:
				case SHADER_PARAM::INT_1:
				case SHADER_PARAM::INT_2:
				case SHADER_PARAM::INT_3:
				{
					int data;
					material->GetData(vectorParam[i].type, &data);
					if (CParamGUI::render_int(name, &data))
					{
						material->SetData(vectorParam[i].type, &data);
					}
				}
					break;
				case SHADER_PARAM::FLOAT_0:
				case SHADER_PARAM::FLOAT_1:
				case SHADER_PARAM::FLOAT_2:
				case SHADER_PARAM::FLOAT_3:
				{
					float data;
					material->GetData(vectorParam[i].type, &data);
					if (CParamGUI::render_float(name, &data))
					{
						material->SetData(vectorParam[i].type, &data);
					}
				}
					break;
				case SHADER_PARAM::VEC2_0:
				case SHADER_PARAM::VEC2_1:
				case SHADER_PARAM::VEC2_2:
				case SHADER_PARAM::VEC2_3:
				{
					Vec2 data;
					material->GetData(vectorParam[i].type, &data);
					if (CParamGUI::render_vec2(name, &data))
					{
						material->SetData(vectorParam[i].type, &data);
					}
				}
					break;
				case SHADER_PARAM::VEC4_0:
				case SHADER_PARAM::VEC4_1:
				case SHADER_PARAM::VEC4_2:
				case SHADER_PARAM::VEC4_3:
				{
					Vector4 data;
					material->GetData(vectorParam[i].type, &data);
					if (CParamGUI::render_vec4(name, &data))
					{
						material->SetData(vectorParam[i].type, &data);
					}
				}
					break;
				case SHADER_PARAM::MATRIX_0:
				case SHADER_PARAM::MATRIX_1:
				case SHADER_PARAM::MATRIX_2:
				case SHADER_PARAM::MATRIX_3:
					break;
				case SHADER_PARAM::TEXTURE_0:
				case SHADER_PARAM::TEXTURE_1:
				case SHADER_PARAM::TEXTURE_2:
				case SHADER_PARAM::TEXTURE_3:
				case SHADER_PARAM::TEXTURE_4:
				case SHADER_PARAM::TEXTURE_5:
				case SHADER_PARAM::TEXTURE_6:
				case SHADER_PARAM::TEXTURE_7:
				case SHADER_PARAM::TEXTURE_ARR_0:
				case SHADER_PARAM::TEXTURE_ARR_1:
				case SHADER_PARAM::TEXTURE_CUBE_0:
				case SHADER_PARAM::TEXTURE_CUBE_1:
				{
					CTexture* tex = nullptr;
					material->GetData(vectorParam[i].type, &tex);

					if (CParamGUI::render_texture(name, tex, this, (GUI_CALLBACK)&CMaterialGUI::SelectTexture))
					{
						m_selectedTextureParam = vectorParam[i].type;
					}
				}
					break;
				}
			}
		}

		End();
	}
}

void CMaterialGUI::SetShader(DWORD_PTR _shaderName, DWORD_PTR _data)
{
	CMaterial* material = (CMaterial*)GetTarget();

	string name = (const char*)_shaderName;
	wstring key = wstring(name.begin(), name.end());
	Ptr<CGraphicsShader> shader = CResourceManager::GetInst()->FindRes<CGraphicsShader>(key);
	material->SetShader(shader);
}

void CMaterialGUI::SelectTexture(DWORD_PTR _textureName, DWORD_PTR _data)
{
	string name = (const char*)_textureName;
	wstring key = wstring(name.begin(), name.end());

	CTexture* tex = CResourceManager::GetInst()->FindRes<CTexture>(key).Get();
	assert(tex);

	CMaterial* material = (CMaterial*)GetTarget();
	material->SetData(m_selectedTextureParam, tex);
}
