#pragma once

class SceneConfig;
class Scene;

class SceneConverter :
    public Singleton<SceneConverter>
{
    SINGLE(SceneConverter)
private :
    vector<SceneConfig*>    m_vectorSceneConfig;
    SceneConfig*            m_curSceneConfig;
    SceneConfig*            m_nextSceneConfig;
    bool                    m_bMove;
    bool                    m_bFinish; // 전투가 있었다가 종료된 상태
    bool                    m_bFirst;
    bool                    m_bFirstAdd;
    bool                    m_bTool;

public :
    void init();
    void progress();

    void SetMove(bool _bMove) { m_bMove = _bMove; }
    void SetFinish(bool _finish) { m_bFinish = _finish; }
    bool IsMove() { return m_bMove; }

    void AddSceneConfig(SceneConfig* _scene) { m_vectorSceneConfig.push_back(_scene); }
    void MoveNextScene(SceneConfig* _scene)
    {
        m_bMove = true;
        m_nextSceneConfig = _scene;
    }

    const vector<SceneConfig*>& GetSceneConfigurations() { return m_vectorSceneConfig; }
    SceneConfig* GetCurSceneConfig() { return m_curSceneConfig; }
    SceneConfig* FindSceneByName(const string& _name);

private :
    void CheckBattlePhase();
    void AppearMonster();
    void ShiftPosition();
    void ListInit();
    void ForceQuitBattle();
};

