#pragma once
#include "CTexture.h"

class CConstBuffer;

class CDevice :
	public CSingleton<CDevice>
{
	SINGLE(CDevice);

private :
	CConstBuffer* m_arrCB[(UINT)CONST_BUFFER::END];
	ComPtr<ID3D11RasterizerState> m_arrRS[(UINT)RASTERIZER_TYPE::END];
	ComPtr<ID3D11BlendState> m_arrBS[(UINT)BLEND_TYPE::END];
	ComPtr<ID3D11DepthStencilState> m_arrDS[(UINT)DEPTH_STENCIL_TYPE::END];
	ComPtr<ID3D11SamplerState> m_arrSampler[2];

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;

	ComPtr<IDXGISwapChain> m_swapChain;
	Ptr<CTexture> m_renderTargetTexture;
	Ptr<CTexture> m_depthStencilTexture;

    HWND m_hWnd;
    bool m_bWindowed;
    Vec2 m_renderResolution;

public:
    int init(HWND _hOutputWnd, Vec2 _renderResolution, bool _bWindowed);
    void Present();

    ComPtr<ID3D11Device> GetDevice() { return m_device; }
    ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_context; }
    ComPtr<IDXGISwapChain> GetSwapChain() { return m_swapChain; }
    const CConstBuffer* GetCB(CONST_BUFFER _type) { return m_arrCB[(UINT)_type]; }
    Vec2 GetRenderResolution() { return m_renderResolution; }

    void SetRS(RASTERIZER_TYPE _type) { CONTEXT->RSSetState(m_arrRS[(UINT)_type].Get()); }
    void SetBS(BLEND_TYPE _type, Vec4 _BlendFactor = Vec4(0.f, 0.f, 0.f, 0.f), UINT _SampleMask = 0xffffffff)
    {
        CONTEXT->OMSetBlendState(m_arrBS[(UINT)_type].Get(), _BlendFactor, _SampleMask);
    }
    void SetDS(DEPTH_STENCIL_TYPE _type, UINT _stencilRef = 0) { CONTEXT->OMSetDepthStencilState(m_arrDS[(UINT)_type].Get(), _stencilRef); }

private:
    int CreateSwapChain();
    void CreateConstBuffer();
    void CreateSampler();
    void CreateRasterizerState();

    void CreateBlendState();
    void CreateDepthStencilState();
};

