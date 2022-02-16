#pragma once
#include "CEntity.h"

enum class STRUCTURED_TYPE
{
    READONLY,
    READ_WRITE,
    DUAL
};

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer> m_SB_CPU_Read;
    ComPtr<ID3D11Buffer> m_SB_CPU_Write;
    ComPtr<ID3D11Buffer> m_SB_RW;
    ComPtr<ID3D11ShaderResourceView> m_SRV;
    ComPtr<ID3D11UnorderedAccessView> m_UAV;
    D3D11_BUFFER_DESC m_desc;

    UINT m_elementSize;
    UINT m_elementCount;

    UINT m_recentRegisterNum;

    STRUCTURED_TYPE m_type;

public:
    void Create(STRUCTURED_TYPE _type, UINT _elementSize, UINT _elementCount, void* _systemData = nullptr);

    void SetData(void* _sysMem, UINT _elementCount) const;
    void GetData(void* _dest) const;

    void UpdateData(UINT _registerNum, SHADER_STAGE _stage = SHADER_STAGE::ALL);
    void UpdateDataRW(UINT _registerNum);
    void Clear(SHADER_STAGE _stage = SHADER_STAGE::ALL);
    void ClearRW();

    UINT GetElementCount() { return m_elementCount; }
    UINT GetElementSize() { return m_elementSize; }
    UINT GetBufferSize() { return m_elementCount * m_elementSize; }

    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    virtual ~CStructuredBuffer();
};

