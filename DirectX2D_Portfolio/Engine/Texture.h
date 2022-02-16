#pragma once
#include "Res.h"
#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

class Texture :
    public Res
{
private :
    ScratchImage                      m_image;
    ComPtr<ID3D11Texture2D>           m_texture2D;
    ComPtr<ID3D11ShaderResourceView>  m_shaderResourceView;
    ComPtr<ID3D11UnorderedAccessView> m_unorderedAccessView;
    ComPtr<ID3D11RenderTargetView>    m_renderTargetView;
    ComPtr<ID3D11DepthStencilView>    m_depthStencilView;
    D3D11_TEXTURE2D_DESC              m_desc;

private :
    virtual int Load(const wstring& _filePath);
    void Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
    void Create(ComPtr<ID3D11Texture2D> _texture);

public :
    void UpdateData(SHADER_STAGE _shaderStage, UINT _registerNum);
    void UpdateRWData(UINT _registerNum);
    static void Clear(UINT _registerNum);
    static void ClearRW(UINT _registerNum);

    float Width() { return (float)m_desc.Width; }
    float Height() { return (float)m_desc.Height; }

    ComPtr<ID3D11Texture2D> GetTexture2D() { return m_texture2D; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_shaderResourceView; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_unorderedAccessView; }
    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_renderTargetView; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_depthStencilView; }

    CLONE_DISABLE(Texture)

public :
    Texture();
    Texture(bool _bDefault);
    virtual ~Texture();
    friend class ResourceManager;
};

