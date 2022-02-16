#pragma once
#include "CEntity.h"
#include "Ptr.h"
#include "CTexture.h"

class CMRT :
    public CEntity
{
private:
    Ptr<CTexture> m_arrRenderTargetTexture[8];
    ID3D11RenderTargetView* m_arrRenderTargetView[8];
    Ptr<CTexture> m_depthStencilTexture;
    UINT m_renderTargetCount;
    D3D11_VIEWPORT m_viewPort;
    bool m_bRelay;

public:
    void Create(Ptr<CTexture>* _renderTarget, UINT _count, Ptr<CTexture> _depthStencil);
    void SetDSRelay(bool _relay) { m_bRelay = _relay; }
    void OMSet();
    void Clear();

    Ptr<CTexture> GetRenderTarget(UINT _index)
    {
        assert(8 > _index);
        return m_arrRenderTargetTexture[_index];
    }
    UINT GetRenderTargetCount() { return m_renderTargetCount; }

    const D3D11_VIEWPORT& GetViewPort() { return m_viewPort; }

    CLONE_DISABLE(CMRT)

public:
    CMRT();
    ~CMRT();
};

