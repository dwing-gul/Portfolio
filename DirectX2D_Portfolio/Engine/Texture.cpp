#include "pch.h"
#include "Texture.h"
#include "Device.h"

Texture::Texture() :
    Res(RESOURCE_TYPE::TEXTURE),
    m_image{},
    m_desc{}
{
}

Texture::Texture(bool _bDefault) :
    Res(RESOURCE_TYPE::TEXTURE, _bDefault),
    m_image{},
    m_desc{}
{
}

Texture::~Texture()
{
}

int Texture::Load(const wstring& _filePath)
{
    wchar_t buffer[50] = {};
    _wsplitpath_s(_filePath.c_str(), 0, 0, 0, 0, 0, 0, buffer, 50);
    wstring ext = buffer;

    HRESULT hReturn = S_OK;

    wstring file = _filePath;

    if (ext == L".dds" || ext == L".DDS")
    {
        hReturn = LoadFromDDSFile(_filePath.c_str(), DDS_FLAGS_NONE, nullptr, m_image);
    }
    else if (ext == L".tga" || ext == L".TGA")
    {
        hReturn = LoadFromTGAFile(_filePath.c_str(), nullptr, m_image);
    }
    else
    {
        hReturn = LoadFromWICFile(_filePath.c_str(), WIC_FLAGS_NONE, nullptr, m_image);
    }

    if (FAILED(hReturn))
    {
        wsprintf(buffer, L"에러 코드 : %d", hReturn);
        MessageBox(nullptr, buffer, L"텍스쳐 로딩 실패", MB_OK);
        return hReturn;
    }

    hReturn = CreateShaderResourceView(DEVICE.Get(), m_image.GetImages(), m_image.GetImageCount(), m_image.GetMetadata(), m_shaderResourceView.GetAddressOf());

    if (FAILED(hReturn))
    {
        wsprintf(buffer, L"에러코드 : %d", hReturn);
        MessageBox(nullptr, buffer, L"ShaderResourceView 생성 실패", MB_OK);
        return E_FAIL;
    }

    m_shaderResourceView->GetResource((ID3D11Resource**)m_texture2D.GetAddressOf());
    m_texture2D->GetDesc(&m_desc);
        
    return S_OK;
}

void Texture::Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag)
{
    m_desc.Width = _width;
    m_desc.Height = _height;
    m_desc.MipLevels = 1;
    m_desc.ArraySize = 1;
    m_desc.Format = _format;

    m_desc.SampleDesc.Count = 1;
    m_desc.SampleDesc.Quality = 0;

    m_desc.BindFlags = _bindFlag;
    m_desc.Usage = D3D11_USAGE_DEFAULT;

    if (FAILED(DEVICE->CreateTexture2D(&m_desc, nullptr, m_texture2D.GetAddressOf())))
    {
        assert(nullptr);
    }

    HRESULT result = 0;
    
    if (D3D11_BIND_DEPTH_STENCIL & m_desc.BindFlags)
    {
        result = DEVICE->CreateDepthStencilView(m_texture2D.Get(), 0, m_depthStencilView.GetAddressOf());

        if (FAILED(result))
        {
            assert(nullptr);
        }
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & m_desc.BindFlags)
        {
            result = DEVICE->CreateRenderTargetView(m_texture2D.Get(), 0, m_renderTargetView.GetAddressOf());

            if (FAILED(result))
            {
                assert(nullptr);
            }
        }

        if (D3D11_BIND_SHADER_RESOURCE & m_desc.BindFlags)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
            desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipLevels = 1;
            desc.Texture2D.MostDetailedMip = 0;

            result = DEVICE->CreateShaderResourceView(m_texture2D.Get(), &desc, m_shaderResourceView.GetAddressOf());

            if (FAILED(result))
            {
                assert(nullptr);
            }
        }

        if (D3D11_BIND_UNORDERED_ACCESS & m_desc.BindFlags)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
            desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            result = DEVICE->CreateUnorderedAccessView(m_texture2D.Get(), &desc, m_unorderedAccessView.GetAddressOf());

            if (FAILED(result))
            {
                assert(nullptr);
            }
        }
    }
}

void Texture::Create(ComPtr<ID3D11Texture2D> _texture)
{
    m_texture2D = _texture;
    m_texture2D->GetDesc(&m_desc);

    HRESULT result = 0;

    if (D3D11_BIND_DEPTH_STENCIL & m_desc.BindFlags)
    {
        result = DEVICE->CreateDepthStencilView(m_texture2D.Get(), 0, m_depthStencilView.GetAddressOf());

        if (FAILED(result))
        {
            assert(nullptr);
        }
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & m_desc.BindFlags)
        {
            result = DEVICE->CreateRenderTargetView(m_texture2D.Get(), 0, m_renderTargetView.GetAddressOf());

            if (FAILED(result))
            {
                assert(nullptr);
            }
        }

        if (D3D11_BIND_SHADER_RESOURCE & m_desc.BindFlags)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
            desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipLevels = 1;
            desc.Texture2D.MostDetailedMip = 0;

            result = DEVICE->CreateShaderResourceView(m_texture2D.Get(), &desc, m_shaderResourceView.GetAddressOf());

            if (FAILED(result))
            {
                assert(nullptr);
            }
        }

        if (D3D11_BIND_UNORDERED_ACCESS & m_desc.BindFlags)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
            desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            result = DEVICE->CreateUnorderedAccessView(m_texture2D.Get(), &desc, m_unorderedAccessView.GetAddressOf());

            if (FAILED(result))
            {
                assert(nullptr);
            }
        }
    }
}

void Texture::UpdateData(SHADER_STAGE _shaderStage, UINT _registerNum)
{
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Vertex)
        CONTEXT->VSSetShaderResources(_registerNum, 1, m_shaderResourceView.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Hull)
        CONTEXT->HSSetShaderResources(_registerNum, 1, m_shaderResourceView.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Domain)
        CONTEXT->DSSetShaderResources(_registerNum, 1, m_shaderResourceView.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Geometry)
        CONTEXT->GSSetShaderResources(_registerNum, 1, m_shaderResourceView.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Pixel)
        CONTEXT->PSSetShaderResources(_registerNum, 1, m_shaderResourceView.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Compute)
        CONTEXT->CSSetShaderResources(_registerNum, 1, m_shaderResourceView.GetAddressOf());
}

void Texture::UpdateRWData(UINT _registerNum)
{
    assert(m_unorderedAccessView);
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_registerNum, 1, m_unorderedAccessView.GetAddressOf(), &i);
}

void Texture::Clear(UINT _registerNum)
{
    ID3D11ShaderResourceView* SRV = nullptr;
    CONTEXT->VSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->HSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->DSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->GSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->PSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->CSSetShaderResources(_registerNum, 1, &SRV);
}

void Texture::ClearRW(UINT _registerNum)
{
    ID3D11UnorderedAccessView* UAV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_registerNum, 1, &UAV, &i);
}
