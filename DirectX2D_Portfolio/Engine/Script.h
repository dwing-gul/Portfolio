#pragma once
#include "Component.h"

class Script :
    public Component
{
private :
    UINT m_scriptType;

public :
    virtual void finalUpdate() final {};
    virtual void render() {};
    static void CreateObject(GameObject* _newObj, Vector3 _worldPos, int _layerIndex = 0);
    static void CreateObject(Scene* _scene, GameObject* _newObj, Vector3 _worldPos, int _layerIndex = 0);
    static void DeleteObject(GameObject* _target);
    static void AddChild(GameObject* _parent, GameObject* _child);
    static void DisconnectWithParent(GameObject* _child);
    static void Instantiate(Ptr<Prefab> _prefab, Vector3 _worldPos, int _layerIndex = 0);

    UINT GetScriptType() { return m_scriptType; }

    virtual void SaveToScene(FILE* _file) {}
    virtual void LoadFromScene(FILE* _file) {}

    virtual Script* Clone() = 0;

public :
    Script(UINT _type);
    ~Script();
};

