#pragma once
#include "Texture.h"

class ConstBuffer;

class Device :
	public Singleton<Device>
{
	SINGLE(Device)
private :
	ConstBuffer*					m_arrCB[(UINT)CONST_BUFFER::END];
	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RASTERIZER_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BLEND_TYPE::END];
	ComPtr<ID3D11DepthStencilState> m_arrDS[(UINT)DEPTH_STENCIL_TYPE::END];
	ComPtr<ID3D11SamplerState>		m_arrSampler[2];

	ComPtr<ID3D11Device>			m_device;
	ComPtr<ID3D11DeviceContext>		m_context;

	ComPtr<IDXGISwapChain>			m_swapChain;
	Ptr<Texture>					m_renderTargetTexture;
	Ptr<Texture>					m_depthStencilTexture;

	D3D11_VIEWPORT					m_viewPort;

	HWND							m_hWnd;
	bool							m_bWindowed;
	Vector2							m_renderResolution;

public :
	int init(HWND _hOutPutWnd, Vector2 _renderResolution, bool _bWindowed);
	void ClearTarget();
	void Present();

	ComPtr<ID3D11Device> GetDevice() { return m_device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_context; }
	const ConstBuffer* GetCB(CONST_BUFFER _type) { return m_arrCB[(UINT)_type]; }
	Vector2 GetRenderResolution() { return m_renderResolution; }

	void SetRS(RASTERIZER_TYPE _type) { CONTEXT->RSSetState(m_arrRS[(UINT)_type].Get()); }
	void SetBS(BLEND_TYPE _type, Vector4 _blendFactor = Vector4(0.f, 0.f, 0.f, 0.f), UINT _sampleMask = 0xffffffff)
	{
		CONTEXT->OMSetBlendState(m_arrBS[(UINT)_type].Get(), _blendFactor, _sampleMask);
	}
	void SetDS(DEPTH_STENCIL_TYPE _type, UINT _stencilRef = 0) { CONTEXT->OMSetDepthStencilState(m_arrDS[(UINT)_type].Get(), _stencilRef); }

private :
	int CreateSwapChain();
	int CreateView();
	void CreateViewPort();
	void CreateConstBuffer();
	void CreateSampler();
	void CreateRasterizerState();
	void CreateBlendState();
	void CreateDepthStencilState();
};

