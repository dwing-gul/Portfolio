#include "pch.h"
#include "CMRT.h"
#include "CDevice.h"

CMRT::CMRT() :
	m_arrRenderTargetView{},
	m_viewPort{},
	m_renderTargetCount(0),
	m_bRelay(false)
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(Ptr<CTexture>* _renderTarget, UINT _count, Ptr<CTexture> _depthStencil)
{
	assert(0 < _count && _count <= 8);

	m_renderTargetCount = _count;

	for (UINT i = 0; i < m_renderTargetCount; ++i)
	{
		m_arrRenderTargetTexture[i] = _renderTarget[i];
		m_arrRenderTargetView[i] = m_arrRenderTargetTexture[i]->GetRTV().Get();
	}

	m_depthStencilTexture = _depthStencil;

	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;

	m_viewPort.Width = _renderTarget[0]->Width();
	m_viewPort.Height = _renderTarget[0]->Height();

	m_viewPort.MinDepth = 0.f;
	m_viewPort.MaxDepth = 1.f;
}

void CMRT::OMSet()
{
	if (nullptr != m_depthStencilTexture)
	{
		CONTEXT->OMSetRenderTargets(m_renderTargetCount, m_arrRenderTargetView, m_depthStencilTexture->GetDSV().Get());
	}
	else
	{
		ComPtr<ID3D11DepthStencilView> DSV = nullptr;

		if (m_bRelay)
		{
			CONTEXT->OMGetRenderTargets(0, nullptr, DSV.GetAddressOf());
		}

		CONTEXT->OMSetRenderTargets(m_renderTargetCount, m_arrRenderTargetView, DSV.Get());
	}

	CONTEXT->RSSetViewports(1, &m_viewPort);
}

void CMRT::Clear()
{
	float arr[4] = { 0.f, 0.f, 0.f, 1.f };

	for (UINT i = 0; i < m_renderTargetCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_arrRenderTargetView[i], arr);
	}

	if (nullptr != m_depthStencilTexture)
	{
		CONTEXT->ClearDepthStencilView(m_depthStencilTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}
