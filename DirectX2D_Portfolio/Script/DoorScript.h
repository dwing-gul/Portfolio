#pragma once
#include <Engine/Script.h>

enum class DOOR_TYPE
{
    ENTERANCE,
    EXIT,
    ROOM_ENTERANCE,
    ROOM_EXIT,
    END
};

class GameObject;

class DoorScript :
    public Script
{
private :
    GameObject*     m_dest;
    wstring         m_destName;
    DOOR_TYPE       m_type;

    bool m_bNext;
    bool m_bEnter;
    bool m_bSound;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetDoorType(DOOR_TYPE _type) { m_type = _type; }
    void SetDestination(GameObject* _dest) { m_dest = _dest; }
    void SetDestName(const wstring& _name) { m_destName = _name; }

    DOOR_TYPE GetDoorType() { return m_type; }
    GameObject* GetDestination() { return m_dest; }
    const wstring& GetDestName() { return m_destName; }
    bool IsSound() { return m_bSound; }
    bool IsEnter() { return m_bEnter; }
    bool IsNext() { return m_bNext; }

    void EnterTheDoor(vector<GameObject*>& _party);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(DoorScript)

public:
    DoorScript();
    ~DoorScript();
};
