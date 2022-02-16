#pragma once
#include "CRes.h"
#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

class CTexture :
    public CRes
{
private:
    ScratchImage m_Image;
    ComPtr<ID3D11Texture2D> m_texture2D;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;
    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    D3D11_TEXTURE2D_DESC m_desc;

private:
    virtual int Load(const wstring& _filePath);
    int Load(const wstring& _filePath, int _mipLevel);
    void Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
    void Create(ComPtr<ID3D11Texture2D> _texture);

public:
    int CreateArrayTexture(const vector<Ptr<CTexture>>& _vectorTexture, int _mipLevel = 1);
    void UpdateData(SHADER_STAGE _shaderStage, UINT _registerNum);
    void UpdateRWData(UINT _registerNum);
    static void Clear(UINT _registerNum);
    static void ClearRW(UINT _registerNum);

    float Width() { return (float)m_desc.Width; }
    float Height() { return (float)m_desc.Height; }

    const D3D11_TEXTURE2D_DESC& GetDesc() const { return m_desc; }

    void* GetSysMem() const { return m_Image.GetPixels(); }
    UINT GetRowPitch() const { return (UINT)m_Image.GetImages()->rowPitch; }
    UINT GetSlicePitch() const { return (UINT)m_Image.GetImages()->slicePitch; }
    UINT GetArrSize() { return m_desc.ArraySize; }

    ComPtr<ID3D11Texture2D> GetTexture2D() { return m_texture2D; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_UAV; }
    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_DSV; }

    virtual void Save(const wstring& _relativePath);

    CLONE_DISABLE(CTexture);

public:
    CTexture();
    CTexture(bool _bDefault);
    ~CTexture();

    friend class CResourceManager;
};
