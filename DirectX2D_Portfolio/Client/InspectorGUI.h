#pragma once
#include "GUI.h"
#include <Script/ScriptManager.h>

class GameObject;
class Res;
class ComponentGUI;
class ResourceGUI;
class ScriptGUI;
class Scene;

enum class INFO_MODE
{
    GAMEOBJECT,
    RESOURCE,
    NONE
};

class InspectorGUI :
    public GUI
{
private :
    Scene*                  m_curScene;
    ComponentGUI*           m_arrComponentGUI[(UINT)COMPONENT_TYPE::END];
    ScriptGUI*              m_arrScriptGUI[(UINT)SCRIPT_TYPE::END];
    GameObject*             m_targetObject;

    ResourceGUI*            m_arrResourceGUI[(UINT)RESOURCE_TYPE::END];
    Res*                    m_targetResource;

    INFO_MODE               m_mode;

public :
    virtual void init();
    virtual void update();

private :
    void update_object();
    void update_resource();
    void AddComponentToObj();
    void SaveMaterial();

public :
    void SetTargetObject(GameObject* _target);
    void SetTargetResource(Res* _target);

    void ResetTargetObject() { m_targetObject = nullptr; m_mode = INFO_MODE::NONE; }

    ComponentGUI* GetArrayComponentGUI(COMPONENT_TYPE _type) { return m_arrComponentGUI[(UINT)_type]; }

public :
    InspectorGUI();
    ~InspectorGUI();
};

