#include "pch.h"
#include "Device.h"
#include "Core.h"
#include "ConstBuffer.h"
#include "ResourceManager.h"

Device::Device() :
	m_arrCB{},
	m_hWnd(nullptr),
	m_renderResolution(Vector2(0.f, 0.f)),
	m_bWindowed(false),
	m_viewPort{}
{
}

Device::~Device()
{
	Safe_Delete_Arr(m_arrCB);
}

int Device::init(HWND _hOutPutWnd, Vector2 _renderResolution, bool _bWindowed)
{
	m_hWnd = _hOutPutWnd;
	m_bWindowed = _bWindowed;
	m_renderResolution = _renderResolution;

	g_global.resolution = m_renderResolution;

	int flag = 0;
#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag, 0, 0, D3D11_SDK_VERSION, &m_device, &featureLevel, &m_context)))
	{
		MessageBox(nullptr, L"장치 객체 생성 오류", L"오류 발생", MB_OK);
		return E_FAIL;
	}

	UINT level = 0;
	m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &level);

	if(0 == level)
	{
		MessageBox(nullptr, L"멀티샘플 지원 불가", L"오류 발생", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSwapChain()))
	{
		return E_FAIL;
	}

	if (FAILED(CreateView()))
	{
		return E_FAIL;
	}

	CreateViewPort();
	CreateConstBuffer();
	CreateSampler();
	CreateRasterizerState();
	CreateBlendState();
	CreateDepthStencilState();

	return S_OK;
}

void Device::ClearTarget()
{
	m_context->OMSetRenderTargets(1, m_renderTargetTexture->GetRTV().GetAddressOf(), m_depthStencilTexture->GetDSV().Get());

	float clearColor[4] = { 0.4f, 0.4f, 0.4f, 1.f };
	m_context->ClearRenderTargetView(m_renderTargetTexture->GetRTV().Get(), clearColor);
	m_context->ClearDepthStencilView(m_depthStencilTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void Device::Present()
{
	m_swapChain->Present(0, 0);
}

int Device::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.BufferDesc.Width = (UINT)m_renderResolution.x;
	desc.BufferDesc.Height = (UINT)m_renderResolution.y;

	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;

	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;

	desc.OutputWindow = m_hWnd;
	desc.Windowed = m_bWindowed;

	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	ComPtr<IDXGIDevice> DXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> adapter = nullptr;
	ComPtr<IDXGIFactory> factory = nullptr;

	m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.GetAddressOf());
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)adapter.GetAddressOf());
	adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());

	if (FAILED(factory->CreateSwapChain(m_device.Get(), &desc, m_swapChain.GetAddressOf())))
	{
		MessageBox(nullptr, L"스왑체인 생성 실패", L"장치 초기화 오류", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

int Device::CreateView()
{
	ComPtr<ID3D11Texture2D> tex = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex.GetAddressOf());
	m_renderTargetTexture = ResourceManager::GetInst()->CreateTexture(L"RenderTargetTexture", tex);
	m_depthStencilTexture = ResourceManager::GetInst()->CreateTexture(L"DepthStencilTexture", (UINT)m_renderResolution.x, (UINT)m_renderResolution.y, 
															DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
	m_context->OMSetRenderTargets(1, m_renderTargetTexture->GetRTV().GetAddressOf(), m_depthStencilTexture->GetDSV().Get());
	return 0;
}

void Device::CreateViewPort()
{
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;

	Vector2 resolution = Core::GetInst()->GetWindowResolution();

	m_viewPort.Width = resolution.x;
	m_viewPort.Height = resolution.y;

	m_viewPort.MinDepth = 0.f;
	m_viewPort.MaxDepth = 1.f;

	m_context->RSSetViewports(1, &m_viewPort);
}

void Device::CreateConstBuffer()
{
	m_arrCB[(UINT)CONST_BUFFER::TRANSFORM] = new ConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::TRANSFORM]->Create(CONST_BUFFER::TRANSFORM, sizeof(tTransform));

	m_arrCB[(UINT)CONST_BUFFER::MATERIAL_PARAM] = new ConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::MATERIAL_PARAM]->Create(CONST_BUFFER::MATERIAL_PARAM, sizeof(tMaterialParam));

	m_arrCB[(UINT)CONST_BUFFER::ANIM2D_INFO] = new ConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::ANIM2D_INFO]->Create(CONST_BUFFER::ANIM2D_INFO, sizeof(tAnim2DInfo));

	m_arrCB[(UINT)CONST_BUFFER::GLOBAL] = new ConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::GLOBAL]->Create(CONST_BUFFER::GLOBAL, sizeof(tGlobalConst));
}

void Device::CreateSampler()
{
	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DEVICE->CreateSamplerState(&desc, m_arrSampler[0].GetAddressOf());

	desc = {};
	desc.Filter = D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DEVICE->CreateSamplerState(&desc, m_arrSampler[1].GetAddressOf());

	ID3D11SamplerState* arrSampler[2] = { m_arrSampler[0].Get(), m_arrSampler[1].Get() };
	CONTEXT->VSSetSamplers(0, 2, arrSampler);
	CONTEXT->HSSetSamplers(0, 2, arrSampler);
	CONTEXT->DSSetSamplers(0, 2, arrSampler);
	CONTEXT->GSSetSamplers(0, 2, arrSampler);
	CONTEXT->PSSetSamplers(0, 2, arrSampler);
	CONTEXT->CSSetSamplers(0, 2, arrSampler);
}

void Device::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};

	m_arrRS[(UINT)RASTERIZER_TYPE::CULL_BACK] = nullptr;

	desc.CullMode = D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RASTERIZER_TYPE::CULL_FRONT].GetAddressOf());

	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RASTERIZER_TYPE::CULL_NONE].GetAddressOf());

	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RASTERIZER_TYPE::WIREFRAME].GetAddressOf()); 
}

void Device::CreateBlendState()
{
	D3D11_BLEND_DESC desc = {};
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BLEND_TYPE::ALPHABLEND].GetAddressOf());
	CONTEXT->OMSetBlendState(m_arrBS[(UINT)BLEND_TYPE::ALPHABLEND].Get(), Vector4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

	desc.AlphaToCoverageEnable = true;

	DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BLEND_TYPE::ALPHABLEND_COVERAGE].GetAddressOf());
	CONTEXT->OMSetBlendState(m_arrBS[(UINT)BLEND_TYPE::ALPHABLEND_COVERAGE].Get(), Vector4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
}

void Device::CreateDepthStencilState()
{
	m_arrDS[(UINT)DEPTH_STENCIL_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC desc = {};
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DEPTH_STENCIL_TYPE::LESS_EQUAL].GetAddressOf());

	desc = {};
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DEPTH_STENCIL_TYPE::GREATER].GetAddressOf());

	desc = {};
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DEPTH_STENCIL_TYPE::NO_TEST].GetAddressOf());

	desc = {};
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DEPTH_STENCIL_TYPE::NO_WRITE].GetAddressOf());

	desc = {};
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
}
