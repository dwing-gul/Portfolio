#pragma once
#include <Engine/CScript.h>

class CLayerUpdateBox :
    public CScript
{
private:
    int     m_layerNumber;

public:

    int GetLayerNumber() { return m_layerNumber; }
    void SetLayerNumber(int _number) { m_layerNumber = _number; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CLayerUpdateBox)
public:
    CLayerUpdateBox();
    ~CLayerUpdateBox();

};