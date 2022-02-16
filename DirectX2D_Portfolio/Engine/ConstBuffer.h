#pragma once
#include "Entity.h"

class ConstBuffer :
    public Entity
{
private :
    ComPtr<ID3D11Buffer> m_CB;
    D3D11_BUFFER_DESC    m_desc;
    CONST_BUFFER         m_type;

public :
    void Create(CONST_BUFFER _type, UINT _bufferSize);
    void SetData(void* _sysMem) const;
    void SetData(void* _sysMem, UINT _size) const;
    void UpdateData(SHADER_STAGE _stage = SHADER_STAGE::ALL) const;
    void Clear(SHADER_STAGE _stage = SHADER_STAGE::ALL) const;

    CLONE_DISABLE(ConstBuffer)

public :
    ConstBuffer();
    virtual ~ConstBuffer();
};

