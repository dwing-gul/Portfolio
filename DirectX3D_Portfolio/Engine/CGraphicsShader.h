#pragma once
#include "CShader.h"

class CGraphicsShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                m_VSBlob;
    ComPtr<ID3DBlob>                m_VSInstBlob;
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11VertexShader>      m_VSInst;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>       m_Layout;
    RENDER_POV                      m_POV;
    D3D11_PRIMITIVE_TOPOLOGY        m_topology;
    RASTERIZER_TYPE                 m_RSType;
    BLEND_TYPE                      m_BlendType;
    DEPTH_STENCIL_TYPE              m_DSType;

public:
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { m_topology = _topology; }
    void SetRSType(RASTERIZER_TYPE _type) { m_RSType = _type; }
    void SetBlendType(BLEND_TYPE _type) { m_BlendType = _type; }
    void SetDSType(DEPTH_STENCIL_TYPE _type) { m_DSType = _type; }
    D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_topology; }
    RENDER_POV GetRenderPOV() { return m_POV; }
    ComPtr<ID3D11VertexShader> GetInstVS() { return m_VSInst; }

    void CreateVertexShader(const wstring& _relativePath, const string& _funcName);
    void CreateHullShader(const wstring& _relativePath, const string& _funcName);
    void CreateDomainShader(const wstring& _relativePath, const string& _funcName);
    void CreateGeometryShader(const wstring& _relativePath, const string& _funcName);
    void CreatePixelShader(const wstring& _relativePath, const string& _funcName);

    virtual void UpdateData();
    void UpdateData_Inst();
    virtual int Load(const wstring& _filePath) { return S_OK; }

    static void Clear();

private:
    CGraphicsShader() :
        CShader(RESOURCE_TYPE::GRAPHICS),
        m_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
        m_POV(RENDER_POV::FORWARD),
        m_DSType(DEPTH_STENCIL_TYPE::LESS),
        m_RSType(RASTERIZER_TYPE::CULL_BACK),
        m_BlendType(BLEND_TYPE::END)
    {
    }

public:
    CGraphicsShader(RENDER_POV _POV);
    virtual ~CGraphicsShader();

    friend class CResourceManager;
};


