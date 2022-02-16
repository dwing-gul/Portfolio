#include "pch.h"
#include "CRenderManager.h"
#include "CDevice.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CKeyManager.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CTileMap.h"
#include "CMRT.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

CRenderManager::CRenderManager() :
	m_arrMRT{},
	m_toolCam(nullptr),
	m_light2DBuffer(nullptr),
	m_light3DBuffer(nullptr),
	m_lightMaterial{},
	m_lightShader{},
	m_lightMergeMaterial(nullptr),
	m_lightMergeShader(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	Safe_Delete_Vec(m_vecCam);
	Safe_Delete_Vec(m_vecLight2D);
	Safe_Delete_Vec(m_vecLight3D);
	Safe_Delete_Arr(m_arrMRT);
	Safe_Delete_Ptr(m_light2DBuffer);
	Safe_Delete_Ptr(m_light3DBuffer);

	for (UINT i = 0; i < (UINT)LIGHT_TYPE::END; ++i)
	{
		Safe_Delete_Ptr(m_lightMaterial[i]);
		Safe_Delete_Ptr(m_lightShader[i]);
	}

	Safe_Delete_Ptr(m_lightMergeMaterial);
	Safe_Delete_Ptr(m_lightMergeShader);
}

void CRenderManager::init()
{
	CreateLightBuffer();

	CreateMRT();

	CreateResource();
}

void CRenderManager::render()
{
	render_init();

	static bool bShow = false;

	if (KEY_TAP(KEY::M))
	{
		bShow = bShow ? false : true;
	}

	if (SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode())
	{
		m_vecCam[0]->SetColliderShow(bShow);
		render_play();
	}
	else
	{
		m_toolCam->SetColliderShow(bShow);
		render_tool();
	}

	m_vecCam.clear();
	render_clear();
}

void CRenderManager::render_play()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortObject();

		if (m_vecCam[i]->GetCameraType() == CAMERA_TYPE::MAIN)
		{
			m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
			m_vecCam[i]->render_deferred();

			render_shadowdepth();

			g_transform.viewMatrix = m_vecCam[i]->GetViewMatrix();
			g_transform.viewMatrixInverse = m_vecCam[i]->GetViewInverseMatrix();
			g_transform.projectionMatrix = m_vecCam[i]->GetProjectionMatrix();
			g_transform.projectionMatrixInverse = m_vecCam[i]->GetProjectionInverseMatrix();

			m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

			for (size_t i = 0; i < m_vecLight3D.size(); ++i)
			{
				m_vecLight3D[i]->render();
			}

			m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

			m_lightMergeMaterial->UpdateData();

			Ptr<CMesh> rectMesh = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
			rectMesh->UpdateData(0);
			rectMesh->render(0);
			m_lightMergeMaterial->Clear();
		}

		m_vecCam[i]->render_forward();
		m_vecCam[i]->render_particle();
		m_vecCam[i]->render_posteffect();
	}
}

void CRenderManager::render_tool()
{
	assert(m_toolCam);

	m_toolCam->SortObject();

	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	m_toolCam->render_deferred();

	render_shadowdepth();

	g_transform.viewMatrix = m_toolCam->GetViewMatrix();
	g_transform.viewMatrixInverse = m_toolCam->GetViewInverseMatrix();
	g_transform.projectionMatrix = m_toolCam->GetProjectionMatrix();
	g_transform.projectionMatrixInverse = m_toolCam->GetProjectionInverseMatrix();

	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->render();
	}

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

	m_lightMergeMaterial->UpdateData();

	Ptr<CMesh> rectMesh = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	rectMesh->UpdateData(0);
	rectMesh->render(0);
	m_lightMergeMaterial->Clear();

	m_toolCam->render_forward();
	m_toolCam->render_particle();
	m_toolCam->render_posteffect();
}

void CRenderManager::render_shadowdepth()
{
	m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH]->OMSet();

	// 광원 시점으로 깊이를 그림
	for (UINT i = 0; i < m_vecLight3D.size(); ++i)
	{
		if (m_vecLight3D[i]->GetLightInfo().lightType != LIGHT_TYPE::DIR)
			continue;

		m_vecLight3D[i]->render_shadowdepth();
	}

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet(); // 메인카메라 깊이로 되돌리기 위해서
}

CCamera* CRenderManager::GetMainCam()
{
	if (SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_toolCam;
	}
}

void CRenderManager::CopyBackBuffer()
{
	Ptr<CTexture> renderTarget = CResourceManager::GetInst()->FindRes<CTexture>(L"RenderTarget");
	CONTEXT->CopyResource(m_postEffectTarget->GetTexture2D().Get(), renderTarget->GetTexture2D().Get());
}

void CRenderManager::render_init()
{
	g_global.light2DCount = (int)m_vecLight2D.size();
	g_global.light3DCount = (int)m_vecLight3D.size();

	const CConstBuffer* GlobalBuffer = CDevice::GetInst()->GetCB(CONST_BUFFER::GLOBAL);
	GlobalBuffer->SetData(&g_global);
	GlobalBuffer->UpdateData();

	static vector<tLightInfo> vectorLight2DInfo;
	static vector<tLightInfo> vectorLight3DInfo;
	vectorLight2DInfo.clear();
	vectorLight3DInfo.clear();

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vectorLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	if (m_light2DBuffer->GetElementCount() < vectorLight2DInfo.size())
	{
		m_light2DBuffer->Create(STRUCTURED_TYPE::READONLY, m_light2DBuffer->GetElementSize(), m_light2DBuffer->GetElementCount() * 2);
	}

	m_light2DBuffer->SetData(vectorLight2DInfo.data(), (UINT)vectorLight2DInfo.size());
	m_light2DBuffer->UpdateData(51);

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		vectorLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
	}

	if (m_light3DBuffer->GetElementCount() < vectorLight3DInfo.size())
	{
		m_light3DBuffer->Create(STRUCTURED_TYPE::READONLY, m_light3DBuffer->GetElementSize(), m_light3DBuffer->GetElementCount() * 2);
	}

	m_light3DBuffer->SetData(vectorLight3DInfo.data(), (UINT)vectorLight3DInfo.size());
	m_light3DBuffer->UpdateData(52);

	ClearMRT();

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
}

void CRenderManager::render_clear()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}

void CRenderManager::CreateLightBuffer()
{
	m_light2DBuffer = new CStructuredBuffer;
	m_light2DBuffer->Create(STRUCTURED_TYPE::READONLY, sizeof(tLightInfo), 5);

	m_light3DBuffer = new CStructuredBuffer;
	m_light3DBuffer->Create(STRUCTURED_TYPE::READONLY, sizeof(tLightInfo), 5);
}

void CRenderManager::CreateMRT()
{
	Vec2 renderResolution = CDevice::GetInst()->GetRenderResolution();

	{
		ComPtr<ID3D11Texture2D> tex = nullptr;
		ComPtr<IDXGISwapChain> swapChain = CDevice::GetInst()->GetSwapChain();
		Vec2 renderResolution = CDevice::GetInst()->GetRenderResolution();

		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex.GetAddressOf());

		Ptr<CTexture> arrTargetTextrue[8] = { CResourceManager::GetInst()->CreateTexture(L"SwapChainTargetTexture", tex) };
		Ptr<CTexture> depthStencilTexture = CResourceManager::GetInst()->CreateTexture(L"DepthStencilTexture", (UINT)renderResolution.x, (UINT)renderResolution.y,
			DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetName(L"SwapChain MRT");
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(arrTargetTextrue, 1, depthStencilTexture);
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
	}

	{
		Ptr<CTexture> colorTarget = CResourceManager::GetInst()->CreateTexture(L"ColorTargetTexture", (UINT)renderResolution.x, (UINT)renderResolution.y,
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> normalTarget = CResourceManager::GetInst()->CreateTexture(L"NormalTargetTexture", (UINT)renderResolution.x, (UINT)renderResolution.y,
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> positionTarget = CResourceManager::GetInst()->CreateTexture(L"PositionTargetTexture", (UINT)renderResolution.x, (UINT)renderResolution.y,
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> arrTargetTextrue[8] = { colorTarget, normalTarget, positionTarget };

		m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetName(L"Deferred MRT");
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetDSRelay(true);
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(arrTargetTextrue, 3, nullptr);
	}

	{
		Ptr<CTexture> diffuseTarget = CResourceManager::GetInst()->CreateTexture(L"DiffuseTargetTexture", (UINT)renderResolution.x, (UINT)renderResolution.y,
			DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> specularTarget = CResourceManager::GetInst()->CreateTexture(L"SpecularTargetTexture", (UINT)renderResolution.x, (UINT)renderResolution.y,
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> arrTargetTextrue[8] = { diffuseTarget, specularTarget };

		m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetName(L"Light MRT");
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetDSRelay(true);
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(arrTargetTextrue, 2, nullptr);
	}

	{
		POINT shadowMapResolution = { 4096, 4096 };

		Ptr<CTexture> shadowMapTex = CResourceManager::GetInst()->CreateTexture(L"ShadowDepthTargetTex", shadowMapResolution.x, shadowMapResolution.y,
			DXGI_FORMAT_R32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> shadowMapDepthTex = CResourceManager::GetInst()->CreateTexture(L"ShadowDepthDepthTex", shadowMapResolution.x, shadowMapResolution.y,
			DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL);

		Ptr<CTexture> arrTex[8] = { shadowMapTex , };
		Vec4 arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH]->SetName(L"Shadow Depth MRT");
		m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH]->Create(arrTex, 1, shadowMapDepthTex);
	}

	Vec2 resolution = CDevice::GetInst()->GetRenderResolution();
	m_postEffectTarget = CResourceManager::GetInst()->CreateTexture(L"PostEffectTarget", (UINT)resolution.x, (UINT)resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_BIND_SHADER_RESOURCE);
}

void CRenderManager::ClearMRT()
{
	for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
	{
		if (nullptr != m_arrMRT[i])
		{
			m_arrMRT[i]->Clear();
		}
	}
}

void CRenderManager::CreateResource()
{
	m_lightShader[(UINT)LIGHT_TYPE::DIR] = new CGraphicsShader(RENDER_POV::LIGHT);
	m_lightShader[(UINT)LIGHT_TYPE::DIR]->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	m_lightShader[(UINT)LIGHT_TYPE::DIR]->SetBlendType(BLEND_TYPE::ONE_ONE);
	m_lightShader[(UINT)LIGHT_TYPE::DIR]->CreateVertexShader(L"shader\\light.fx", "VS_DirLight");
	m_lightShader[(UINT)LIGHT_TYPE::DIR]->CreatePixelShader(L"shader\\light.fx", "PS_DirLight");

	m_lightMaterial[(UINT)LIGHT_TYPE::DIR] = new CMaterial(true);
	m_lightMaterial[(UINT)LIGHT_TYPE::DIR]->SetShader(m_lightShader[(UINT)LIGHT_TYPE::DIR]);
	m_lightMaterial[(UINT)LIGHT_TYPE::DIR]->SetData(SHADER_PARAM::TEXTURE_0, CResourceManager::GetInst()->FindRes<CTexture>(L"NormalTargetTexture").Get());
	m_lightMaterial[(UINT)LIGHT_TYPE::DIR]->SetData(SHADER_PARAM::TEXTURE_1, CResourceManager::GetInst()->FindRes<CTexture>(L"PositionTargetTexture").Get());

	m_lightShader[(UINT)LIGHT_TYPE::POINT] = new CGraphicsShader(RENDER_POV::LIGHT);
	m_lightShader[(UINT)LIGHT_TYPE::POINT]->SetRSType(RASTERIZER_TYPE::CULL_FRONT);
	m_lightShader[(UINT)LIGHT_TYPE::POINT]->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	m_lightShader[(UINT)LIGHT_TYPE::POINT]->SetBlendType(BLEND_TYPE::ONE_ONE);
	m_lightShader[(UINT)LIGHT_TYPE::POINT]->CreateVertexShader(L"shader\\light.fx", "VS_PointLight");
	m_lightShader[(UINT)LIGHT_TYPE::POINT]->CreatePixelShader(L"shader\\light.fx", "PS_PointLight");

	m_lightMaterial[(UINT)LIGHT_TYPE::POINT] = new CMaterial(true);
	m_lightMaterial[(UINT)LIGHT_TYPE::POINT]->SetShader(m_lightShader[(UINT)LIGHT_TYPE::POINT]);
	m_lightMaterial[(UINT)LIGHT_TYPE::POINT]->SetData(SHADER_PARAM::TEXTURE_0, CResourceManager::GetInst()->FindRes<CTexture>(L"NormalTargetTexture").Get());
	m_lightMaterial[(UINT)LIGHT_TYPE::POINT]->SetData(SHADER_PARAM::TEXTURE_1, CResourceManager::GetInst()->FindRes<CTexture>(L"PositionTargetTexture").Get());

	m_lightMergeShader = new CGraphicsShader(RENDER_POV::LIGHT);
	m_lightMergeShader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	m_lightMergeShader->CreateVertexShader(L"shader\\light.fx", "VS_LightMerge");
	m_lightMergeShader->CreatePixelShader(L"shader\\light.fx", "PS_LightMerge");

	m_lightMergeMaterial = new CMaterial(true);
	m_lightMergeMaterial->SetShader(m_lightMergeShader);
	m_lightMergeMaterial->SetData(SHADER_PARAM::TEXTURE_0, CResourceManager::GetInst()->FindRes<CTexture>(L"ColorTargetTexture").Get());
	m_lightMergeMaterial->SetData(SHADER_PARAM::TEXTURE_1, CResourceManager::GetInst()->FindRes<CTexture>(L"DiffuseTargetTexture").Get());
	m_lightMergeMaterial->SetData(SHADER_PARAM::TEXTURE_2, CResourceManager::GetInst()->FindRes<CTexture>(L"SpecularTargetTexture").Get());
}