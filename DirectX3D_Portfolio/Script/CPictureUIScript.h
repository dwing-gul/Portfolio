#pragma once
#include "CUIScript.h"
class CPictureUIScript :
    public CUIScript
{
private :
    static UINT g_nextPictureID;

private :
    bool m_bProfileChange;
    UINT m_pictureID;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void PlayerChange();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CPictureUIScript)

public:
    CPictureUIScript();
    ~CPictureUIScript();
};

