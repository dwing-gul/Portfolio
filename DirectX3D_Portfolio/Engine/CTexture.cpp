#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"
#include "CPathManager.h"

CTexture::CTexture() :
    CRes(RESOURCE_TYPE::TEXTURE),
    m_Image{},
    m_desc{}
{
}

CTexture::CTexture(bool _bDefault) :
    CRes(RESOURCE_TYPE::TEXTURE, _bDefault),
    m_Image{},
    m_desc{}
{
}

CTexture::~CTexture()
{
}


void CTexture::Save(const wstring& _relativePath)
{
    CaptureTexture(DEVICE, CONTEXT, m_texture2D.Get(), m_Image);

    HRESULT hr = S_OK;

    wstring path = CPathManager::GetInst()->GetContentPath();
    path += _relativePath;

    size_t arrSize = m_Image.GetMetadata().arraySize;

    if (1 < m_desc.MipLevels)
    {
        D3D11_TEXTURE2D_DESC desc = {};
        desc = m_desc;
        desc.MipLevels = 1;

        ComPtr<ID3D11Texture2D> copyTex2D;
        hr = DEVICE->CreateTexture2D(&desc, nullptr, copyTex2D.GetAddressOf());

        for (int i = 0; i < (int)m_desc.ArraySize; ++i)
        {
            UINT destSubResIndex = D3D11CalcSubresource(0, i, 1);
            UINT srcSubResIndex = D3D11CalcSubresource(0, i, m_desc.MipLevels);

            CONTEXT->CopySubresourceRegion(copyTex2D.Get(), destSubResIndex, 0, 0, 0, m_texture2D.Get(), srcSubResIndex, nullptr);
        }

        ScratchImage tempImage;
        CaptureTexture(DEVICE, CONTEXT, copyTex2D.Get(), tempImage);

        if (1 < arrSize)
        {
            SaveToDDSFile(tempImage.GetImages(), arrSize, tempImage.GetMetadata(), DDS_FLAGS::DDS_FLAGS_NONE, path.c_str());
        }
        else
        {
            SaveToWICFile(*(tempImage.GetImages()), WIC_FLAGS_NONE, GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG)), path.c_str());
        }
    }
    else
    {
        if (1 < arrSize)
        {
            SaveToDDSFile(m_Image.GetImages(), arrSize, m_Image.GetMetadata(), DDS_FLAGS::DDS_FLAGS_NONE, path.c_str());
        }
        else
        {
            SaveToWICFile(*(m_Image.GetImages()), WIC_FLAGS_NONE, GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG)), path.c_str());
        }
    }
}

int CTexture::Load(const wstring& _filePath)
{
    return Load(_filePath, 1);
}

int CTexture::Load(const wstring& _filePath, int _mipLevel)
{
    wchar_t buffer[50] = {};
    _wsplitpath_s(_filePath.c_str(), 0, 0, 0, 0, 0, 0, buffer, 50);
    wstring ext = buffer;

    HRESULT hReturn = S_OK;

    if (ext == L".dds" || ext == L".DDS")
    {
        hReturn = LoadFromDDSFile(_filePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
    }
    else if (ext == L".tga" || ext == L".TGA")
    {
        hReturn = LoadFromTGAFile(_filePath.c_str(), nullptr, m_Image);
    }
    else
    {
        hReturn = LoadFromWICFile(_filePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hReturn))
    {
        wsprintf(buffer, L"에러 코드 : %d", hReturn);
        MessageBox(nullptr, buffer, L"텍스쳐 로딩 실패", MB_OK);
        return hReturn;
    }

    if (1 == _mipLevel)
    {
        hReturn = CreateShaderResourceView(DEVICE, m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), m_SRV.GetAddressOf());

        if (FAILED(hReturn))
        {
            wsprintf(buffer, L"에러코드 : %d", hReturn);
            MessageBox(nullptr, buffer, L"ShaderResourceView 생성 실패", MB_OK);
            return E_FAIL;
        }

        m_SRV->GetResource((ID3D11Resource**)m_texture2D.GetAddressOf());
        m_texture2D->GetDesc(&m_desc);

        return S_OK;
    }
    else
    {
        m_desc.Format = m_Image.GetMetadata().format;
        m_desc.ArraySize = UINT(m_Image.GetMetadata().arraySize);
        m_desc.MipLevels = _mipLevel;
        m_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        m_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;


        m_desc.SampleDesc.Count = 1;
        m_desc.SampleDesc.Quality = 0;

        m_desc.Usage = D3D11_USAGE_DEFAULT;
        m_desc.CPUAccessFlags = 0;

        m_desc.Width = UINT(m_Image.GetMetadata().width);
        m_desc.Height = UINT(m_Image.GetMetadata().height);

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
        viewDesc.Format = m_desc.Format;

        DEVICE->CreateTexture2D(&m_desc, nullptr, &m_texture2D);

        for (size_t i = 0; i < m_desc.ArraySize; ++i)
        {
            UINT offset = D3D11CalcSubresource(0, (UINT)i, m_desc.MipLevels);
            CONTEXT->UpdateSubresource(m_texture2D.Get(), offset, nullptr, m_Image.GetImage(0, i, 0)->pixels, UINT(m_Image.GetImage(0, i, 0)->rowPitch), UINT(m_Image.GetImage(0, i, 0)->slicePitch));
        }

        if (1 < m_desc.ArraySize)
        {
            viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            viewDesc.Texture2DArray.MipLevels = m_desc.MipLevels;
            viewDesc.Texture2DArray.MostDetailedMip = 0;
            viewDesc.Texture2DArray.ArraySize = m_desc.ArraySize;
        }
        else
        {
            viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            viewDesc.Texture2D.MipLevels = m_desc.MipLevels;
            viewDesc.Texture2D.MostDetailedMip = 0;
        }

        DEVICE->CreateShaderResourceView(m_texture2D.Get(), &viewDesc, m_SRV.GetAddressOf());

        CONTEXT->GenerateMips(m_SRV.Get());

        return S_OK;
    }
}

void CTexture::Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag)
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

    HRESULT hr = 0;

    if (D3D11_BIND_DEPTH_STENCIL & m_desc.BindFlags)
    {
        hr = DEVICE->CreateDepthStencilView(m_texture2D.Get(), 0, m_DSV.GetAddressOf());

        if (FAILED(hr))
            assert(nullptr);
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & m_desc.BindFlags)
        {
            hr = DEVICE->CreateRenderTargetView(m_texture2D.Get(), 0, m_RTV.GetAddressOf());

            if (FAILED(hr))
                assert(nullptr);
        }

        if (D3D11_BIND_SHADER_RESOURCE & m_desc.BindFlags)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

            SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            SRVDesc.Texture2D.MipLevels = 1;
            SRVDesc.Texture2D.MostDetailedMip = 0;

            hr = DEVICE->CreateShaderResourceView(m_texture2D.Get(), &SRVDesc, m_SRV.GetAddressOf());

            if (FAILED(hr))
                assert(nullptr);
        }

        if (D3D11_BIND_UNORDERED_ACCESS & m_desc.BindFlags)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

            UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            hr = DEVICE->CreateUnorderedAccessView(m_texture2D.Get(), &UAVDesc, m_UAV.GetAddressOf());

            if (FAILED(hr))
                assert(nullptr);
        }
    }
}

void CTexture::Create(ComPtr<ID3D11Texture2D> _texture)
{
    m_texture2D = _texture;

    m_texture2D->GetDesc(&m_desc);

    HRESULT hr = 0;

    if (D3D11_BIND_DEPTH_STENCIL & m_desc.BindFlags)
    {
        hr = DEVICE->CreateDepthStencilView(m_texture2D.Get(), 0, m_DSV.GetAddressOf());

        if (FAILED(hr))
            assert(nullptr);
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & m_desc.BindFlags)
        {
            hr = DEVICE->CreateRenderTargetView(m_texture2D.Get(), 0, m_RTV.GetAddressOf());

            if (FAILED(hr))
                assert(nullptr);
        }

        if (D3D11_BIND_SHADER_RESOURCE & m_desc.BindFlags)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

            SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            SRVDesc.Texture2D.MipLevels = 1;
            SRVDesc.Texture2D.MostDetailedMip = 0;

            hr = DEVICE->CreateShaderResourceView(m_texture2D.Get(), &SRVDesc, m_SRV.GetAddressOf());

            if (FAILED(hr))
                assert(nullptr);
        }

        if (D3D11_BIND_UNORDERED_ACCESS & m_desc.BindFlags)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

            UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            hr = DEVICE->CreateUnorderedAccessView(m_texture2D.Get(), &UAVDesc, m_UAV.GetAddressOf());

            if (FAILED(hr))
                assert(nullptr);
        }
    }
}

int CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& _vectorTexture, int _mipLevel)
{
    m_desc = _vectorTexture[0]->GetDesc();
    m_desc.ArraySize = (UINT)_vectorTexture.size();
    m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    m_desc.MipLevels = _mipLevel;

    HRESULT hr = DEVICE->CreateTexture2D(&m_desc, nullptr, m_texture2D.GetAddressOf());

    if (FAILED(hr))
        return hr;


    for (size_t i = 0; i < _vectorTexture.size(); ++i)
    {
        UINT offset = D3D11CalcSubresource(0, (UINT)i, m_desc.MipLevels);
        CONTEXT->UpdateSubresource(m_texture2D.Get(), offset, nullptr, _vectorTexture[i]->GetSysMem(), _vectorTexture[i]->GetRowPitch(), _vectorTexture[i]->GetSlicePitch());
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    viewDesc.Format = m_desc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    viewDesc.Texture2DArray.MipLevels = m_desc.MipLevels;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.ArraySize = m_desc.ArraySize;

    hr = DEVICE->CreateShaderResourceView(m_texture2D.Get(), nullptr, m_SRV.GetAddressOf());
    CONTEXT->GenerateMips(m_SRV.Get());

    return hr;
}

void CTexture::UpdateData(SHADER_STAGE _shaderStage, UINT _registerNum)
{
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Vertex)
        CONTEXT->VSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Hull)
        CONTEXT->HSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Domain)
        CONTEXT->DSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Geometry)
        CONTEXT->GSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Pixel)
        CONTEXT->PSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
    if ((UINT)_shaderStage & (UINT)SHADER_STAGE::Compute)
        CONTEXT->CSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
}

void CTexture::UpdateRWData(UINT _registerNum)
{
    assert(m_UAV);
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_registerNum, 1, m_UAV.GetAddressOf(), &i);
}

void CTexture::Clear(UINT _registerNum)
{
    ID3D11ShaderResourceView* SRV = nullptr;
    CONTEXT->VSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->HSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->DSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->GSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->PSSetShaderResources(_registerNum, 1, &SRV);
    CONTEXT->CSSetShaderResources(_registerNum, 1, &SRV);
}

void CTexture::ClearRW(UINT _registerNum)
{
    ID3D11UnorderedAccessView* UAV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_registerNum, 1, &UAV, &i);
}
