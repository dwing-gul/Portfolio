#include "pch.h"
#include "CMRTViewGUI.h"
#include <Engine/CRenderManager.h>
#include <Engine/CResourceManager.h>
#include <Engine/CMRT.h>
#include <Engine/CTexture.h>

CMRTViewGUI::CMRTViewGUI() :
	m_renderIndex(0)
{
	SetName("MRTViewGUI");
	Deactivate();
}

CMRTViewGUI::~CMRTViewGUI()
{
	Safe_Delete_Vec(m_vecCopyTexture);
}

void CMRTViewGUI::init()
{
	m_CSCopy = (CCopyTextureShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"CopyTextureShader").Get();
}

void CMRTViewGUI::update()
{
	ImGui::Begin(GetName().c_str(), &IsCancelButton());

	if (!IsCancelButton())
	{
		Deactivate();
	}

	for (UINT i = (UINT)MRT_TYPE::DEFERRED; i < (UINT)MRT_TYPE::END; ++i)
	{
		CMRT* mrt = CRenderManager::GetInst()->GetMRT((MRT_TYPE)i);

		if (nullptr != mrt)
			render_MRT(mrt);
	}

	ImGui::End();

	m_renderIndex = 0;
}

void CMRTViewGUI::render_MRT(CMRT* _mrt)
{
	ImGui::PushID((int)_mrt->GetID());
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.5f, 0.8f, 1.f));
	wstring mrtName = _mrt->GetName();
	string name = string(mrtName.begin(), mrtName.end());
	ImGui::Button(name.c_str());
	ImGui::PopStyleColor(1);
	ImGui::PopID();

	UINT renderTargetCount = _mrt->GetRenderTargetCount();
	for (UINT i = 0; i < renderTargetCount; ++i)
	{
		Ptr<CTexture> renderTargetTexture = _mrt->GetRenderTarget(i);

		assert(nullptr != renderTargetTexture);

		if (m_renderIndex >= m_vecCopyTexture.size())
		{
			CreateCopyTexture();
		}

		CopyRenderTarget(m_vecCopyTexture[m_renderIndex], renderTargetTexture);

		ImVec2 uvMin = ImVec2(0.f, 0.f);
		ImVec2 uvMax = ImVec2(1.f, 1.f);
		ImVec4 tintColor = ImVec4(1.f, 1.f, 1.f, 1.f);
		ImVec4 borderColor = ImVec4(1.f, 1.f, 1.f, 0.5f);

		ImTextureID texID = (ImTextureID)m_vecCopyTexture[m_renderIndex]->GetSRV().Get();
	
		ImGui::Image(texID, ImVec2(150.f, 150.f), uvMin, uvMax, tintColor, borderColor);

		if(i != renderTargetCount - 1)
			ImGui::SameLine();

		++m_renderIndex;
	}
}

void CMRTViewGUI::CreateCopyTexture()
{
	CTexture* texture = CResourceManager::GetInst()->CreateTexture_Tool(150, 150, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS).Get();

	m_vecCopyTexture.push_back(texture);
}

void CMRTViewGUI::CopyRenderTarget(Ptr<CTexture> _dest, Ptr<CTexture> _src)
{
	m_CSCopy->SetDestTexture(_dest);
	m_CSCopy->SetSrcTexture(_src);
	m_CSCopy->IsSampling(true);

	m_CSCopy->Excute();
}
