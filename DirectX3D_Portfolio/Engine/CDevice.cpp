#include "pch.h"
#include "CDevice.h"
#include "CCore.h"
#include "CConstBuffer.h"
#include "CResourceManager.h"

CDevice::CDevice() :
	m_arrCB{},
	m_arrRS{},
	m_arrBS{},
	m_arrDS{},
	m_arrSampler{},
	m_hWnd(nullptr),
	m_bWindowed(false)
{
}

CDevice::~CDevice()
{
	Safe_Delete_Arr(m_arrCB);
}

int CDevice::init(HWND _hOutputWnd, Vec2 _renderResolution, bool _bWindowed)
{
	m_hWnd = _hOutputWnd;
	m_bWindowed = _bWindowed;
	m_renderResolution = _renderResolution;

	g_global.resolution = m_renderResolution;

	int flag = 0;
#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag, 0, 0, D3D11_SDK_VERSION,
		&m_device, &level, &m_context)))
	{
		MessageBox(nullptr, L"장치 객체 생성 오류", L"오류 발생", MB_OK);
		return E_FAIL;
	}

	UINT multisampleQualityLevel = 0;
	m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &multisampleQualityLevel);

	if (multisampleQualityLevel == 0)
	{
		MessageBox(nullptr, L"멀티샘플 지원 불가", L"오류 발생", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSwapChain()))
	{
		return E_FAIL;
	}

	CreateConstBuffer();

	CreateSampler();

	CreateRasterizerState();

	CreateBlendState();

	CreateDepthStencilState();

	return S_OK;
}

void CDevice::Present()
{
	m_swapChain->Present(0, 0);
}

int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.BufferDesc.Width = (UINT)m_renderResolution.x;
	desc.BufferDesc.Height = (UINT)m_renderResolution.y;

	// 화면 갱신 비율
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;

	// 픽셀 포맷
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;

	// 출력 윈도우 핸들
	desc.OutputWindow = m_hWnd;
	desc.Windowed = m_bWindowed;

	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;


	ComPtr<IDXGIDevice>  DXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> Adapter = nullptr;
	ComPtr<IDXGIFactory> Factory = nullptr;

	m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.GetAddressOf());
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)Adapter.GetAddressOf());
	Adapter->GetParent(__uuidof(IDXGIFactory), (void**)Factory.GetAddressOf());

	if (FAILED(Factory->CreateSwapChain(m_device.Get(), &desc, m_swapChain.GetAddressOf())))
	{
		MessageBox(nullptr, L"스왑체인 생성 실패", L"장치 초기화 오류", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CONST_BUFFER::TRANSFORM] = new CConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::TRANSFORM]->Create(CONST_BUFFER::TRANSFORM, sizeof(tTransform));

	m_arrCB[(UINT)CONST_BUFFER::MATERIAL_PARAM] = new CConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::MATERIAL_PARAM]->Create(CONST_BUFFER::MATERIAL_PARAM, sizeof(tMaterialParam));

	m_arrCB[(UINT)CONST_BUFFER::ANIM2D_INFO] = new CConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::ANIM2D_INFO]->Create(CONST_BUFFER::ANIM2D_INFO, sizeof(tAnim2DInfo));

	m_arrCB[(UINT)CONST_BUFFER::GLOBAL] = new CConstBuffer;
	m_arrCB[(UINT)CONST_BUFFER::GLOBAL]->Create(CONST_BUFFER::GLOBAL, sizeof(tGlobalConst));
}

void CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	DEVICE->CreateSamplerState(&desc, m_arrSampler[0].GetAddressOf());

	CONTEXT->VSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->CSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());

	desc = {};
	desc.Filter = D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	DEVICE->CreateSamplerState(&desc, m_arrSampler[1].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->CSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
}

void CDevice::CreateRasterizerState()
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

void CDevice::CreateBlendState()
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

	desc.AlphaToCoverageEnable = true;

	DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BLEND_TYPE::ALPHABLEND_COVERAGE].GetAddressOf());

	desc.AlphaToCoverageEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BLEND_TYPE::ONE_ONE].GetAddressOf());
}

void CDevice::CreateDepthStencilState()
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