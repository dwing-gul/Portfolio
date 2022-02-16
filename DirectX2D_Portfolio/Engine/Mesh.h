#pragma once
#include "Res.h"

class Mesh :
    public Res
{
private :
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    void*                m_vertexSys;
    void*                m_indexSys;
    D3D11_BUFFER_DESC    m_vertexDesc;
    D3D11_BUFFER_DESC    m_indexDesc;
    UINT                 m_indexCount;

public :
    virtual int Load(const wstring& _filePath) { return S_OK; }
    void Create(void* _vertexSys, UINT _vertexBufferSize, void* _indexSys, UINT _indexBufferSize);

    virtual void UpdateData();
    void render();
    void render_particle(UINT _count);

    CLONE_DISABLE(Mesh)

public :
    Mesh();
    ~Mesh();
};

