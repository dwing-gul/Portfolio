#pragma once
#include <Engine/Script.h>

enum class BUTTON_TYPE
{
    CANCEL,
    BUY,
    END
};
class StoreItemScript :
    public Script
{
private :
    BUTTON_TYPE m_type;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetButtonType(BUTTON_TYPE _type) { m_type = _type; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(StoreItemScript)

public:
    StoreItemScript();
    ~StoreItemScript();
};

