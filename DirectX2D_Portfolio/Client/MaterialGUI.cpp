#include "pch.h"
#include "MaterialGUI.h"
#include "ListGUI.h"
#include "ParamGUI.h"
#include "ImGuiManager.h"
#include <Engine/Material.h>
#include <Engine/ResourceManager.h>

MaterialGUI::MaterialGUI() :
	ResourceGUI(RESOURCE_TYPE::MATERIAL),
	m_selectedTextureParam(SHADER_PARAM::TEXTURE_END)
{
}

MaterialGUI::~MaterialGUI()
{
}

void MaterialGUI::update()
{
	if (Start())
	{
		Material* material = (Material*)GetTarget();
		Shader* shader = material->GetShader().Get();

		char shaderName[255] = {};
		GetResourceName(shader, shaderName);

		ImGui::Text("Shader");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Shader", shaderName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Shader"))
		{
			ListGUI* listGUI = (ListGUI*)ImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Shader");
			vector<wstring> vectorName;
			ResourceManager::GetInst()->GetResName(RESOURCE_TYPE::GRAPHICS, vectorName);
			listGUI->SetList(vectorName);
			if (!material->IsDefault())
			{
				listGUI->SetDoubleClickedCallBack(this, (GUI_CALLBACK)&MaterialGUI::SetShader);
				listGUI->Activate();
			}
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
					if (ParamGUI::render_int(name, &data) && !material->IsDefault())
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
					if (ParamGUI::render_float(name, &data) && !material->IsDefault())
					{
						material->SetData(vectorParam[i].type, &data);
					}
				}
					break;
				case SHADER_PARAM::VECTOR2_0:
				case SHADER_PARAM::VECTOR2_1:
				case SHADER_PARAM::VECTOR2_2:
				case SHADER_PARAM::VECTOR2_3:
				{
					Vector2 data;
					material->GetData(vectorParam[i].type, &data);
					if (ParamGUI::render_vector2(name, &data) && !material->IsDefault())
					{
						material->SetData(vectorParam[i].type, &data);
					}
				}
					break;
				case SHADER_PARAM::VECTOR4_0:
				case SHADER_PARAM::VECTOR4_1:
				case SHADER_PARAM::VECTOR4_2:
				case SHADER_PARAM::VECTOR4_3:
				{
					Vector4 data;
					material->GetData(vectorParam[i].type, &data);
					if (ParamGUI::render_vector4(name, &data) && !material->IsDefault())
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
					Texture* tex = nullptr;
					material->GetData(vectorParam[i].type, &tex);

					if (ParamGUI::render_texture(name, tex, this, (GUI_CALLBACK)&MaterialGUI::SelectTexture) && !material->IsDefault())
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

void MaterialGUI::SetShader(DWORD_PTR _shaderName, DWORD_PTR _data)
{
	Material* material = (Material*)GetTarget();

	string name = (const char*)_shaderName;
	wstring key = StringToWString(name);
	Ptr<GraphicsShader> shader = ResourceManager::GetInst()->FindRes<GraphicsShader>(key);
	material->SetShader(shader);
}

void MaterialGUI::SelectTexture(DWORD_PTR _textureName, DWORD_PTR _data)
{
	string name = (const char*)_textureName;
	wstring key = StringToWString(name);

	Texture* tex = ResourceManager::GetInst()->FindRes<Texture>(key).Get();
	assert(tex);

	Material* material = (Material*)GetTarget();
	material->SetData(m_selectedTextureParam, tex);
}
