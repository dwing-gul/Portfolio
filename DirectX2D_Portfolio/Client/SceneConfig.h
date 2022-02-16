#pragma once

class SceneConfig
{
private :
    string m_name;

public:
    virtual void CreateScene() = 0;
    void SetName(const string& _name) { m_name = _name; }
    const string& GetName() { return m_name; }

public:
    SceneConfig();
    virtual ~SceneConfig();
};

