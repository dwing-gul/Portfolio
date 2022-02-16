#pragma once
#include "CGUI.h"
#include <Engine/CCopyTextureShader.h>

class CMRT;
class CTexture;

class CMRTViewGUI :
    public CGUI
{
private :
    vector<CTexture*> m_vecCopyTexture;
    UINT m_renderIndex;

    Ptr<CCopyTextureShader> m_CSCopy;

public :
    virtual void init();
    virtual void update();

private :
    void render_MRT(CMRT* _mrt);
    void CreateCopyTexture();
    void CopyRenderTarget(Ptr<CTexture> _dest, Ptr<CTexture> _src);

public :
    CMRTViewGUI();
    ~CMRTViewGUI();
};

