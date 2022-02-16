#pragma once
#include "CGUI.h"
#include <Script/CScriptManager.h>

class CGameObject;
class CRes;
class CScriptGUI;
class CScene;
class CComponentGUI;
class CResourceGUI;

enum class INFO_MODE
{
    GAMEOBJECT,
    RESOURCE,
    NONE
};

class CInspectorGUI :
    public CGUI
{
private :
    CScene* m_curScene;
    CComponentGUI* m_arrComponentGUI[(UINT)COMPONENT_TYPE::END];
    CScriptGUI* m_arrScriptGUI[(UINT)SCRIPT_TYPE::END];
    CGameObject* m_targetObject;

    CResourceGUI* m_arrResourceGUI[(UINT)RESOURCE_TYPE::END];
    CRes* m_targetResource;

    INFO_MODE m_mode;

public :
    virtual void init();
    virtual void update();

private :
    void update_object();
    void update_resource();
    void AddComponentToObj();
    void SaveMaterial();

public :
    void SetTargetObject(CGameObject* _target);
    void SetTargetResource(CRes* _target);
    void ResetTargetObject() { m_targetObject = nullptr; m_mode = INFO_MODE::NONE; }

    CComponentGUI* GetArrayComponentGUI(COMPONENT_TYPE _type) { return m_arrComponentGUI[(UINT)_type]; }

public :
    CInspectorGUI();
    ~CInspectorGUI();
};

