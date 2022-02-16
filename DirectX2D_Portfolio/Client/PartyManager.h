#pragma once

class GameObject;
class MonsterScript;
class Prefab;

struct tOrder
{
	int			speed;
	GameObject* target;
};

class PartyManager :
	public Singleton<PartyManager>
{
	SINGLE(PartyManager)
private :
	vector<GameObject*> m_vectorPlayerParty;
	vector<GameObject*> m_vectorMonsterParty;
	vector<Prefab*>		m_vectorPlayerWaiting;
	vector<Prefab*>		m_vectorMonsterWaiting;
	vector<tOrder>		m_vectorOrder; // �켱 ������ ����ϱ� ���� ����. ���ǵ�� ������Ʈ�� �������
	GameObject*			m_targetDoor; // � ������ ������
	bool				m_bEnter; // ������ ������
	bool				m_bBattle;
	bool				m_bChangeScene;
	bool				m_bTurn;
	bool				m_bNext; // ���� ���� ��ġ�� �Ѿ�� �Ǵ���
	bool				m_bDoorSound;
	bool				m_bTown;
	bool				m_bFirstTurn;
	int					m_battleSoundChannel;
	int					m_exploreSoundChannel;
	int					m_townSoundChannel;
	int					m_turnCount;
	int					m_startCheck;

public :
	void init();
	void update();

	void SetEnter(bool _enter) { m_bEnter = _enter; }
	void SetBattle(bool _battle) { m_bBattle = _battle; }
	void SetNext(bool _next) { m_bNext = _next; }
	void SetTown(bool _town) { m_bTown = _town; }

	void AddPlayerParty(UINT _position, GameObject* _player);
	void SubPlayerParty(UINT _position) { m_vectorPlayerParty[_position] = nullptr; }
	void ResetPlayerParty();
	const vector<GameObject*>& GetPlayerParty() { return m_vectorPlayerParty; }

	void AddMonsterParty(UINT _position, GameObject* _monster);
	void SubMonsterParty(UINT _position) { m_vectorMonsterParty[_position] = nullptr; }
	void ResetMonsterParty();
	const vector<GameObject*>& GetMonsterParty() { return m_vectorMonsterParty; }

	void SetWaitingMonster(Prefab* _monster) { m_vectorMonsterWaiting.push_back(_monster); }
	vector<Prefab*>& GetWaitingMonster() { return m_vectorMonsterWaiting; }

	void SetWaitingPlayer(Prefab* _player) 
	{
		if (m_vectorPlayerParty.size() > 4)
			return;

		m_vectorPlayerWaiting.push_back(_player); 
	}
	vector<Prefab*>& GetWaitingPlayer() { return m_vectorPlayerWaiting; }

	GameObject* GetDestination();

	bool IsBattle() { return m_bBattle; }
	bool IsNext() { return m_bNext; }
	void StopSoundAll();

private :
	void CalOrder();
	void CheckDoor();
	void CheckScene();
	void BattlePhaseManagement();
	void BattlePhaseSound();
	void ExploreSound();
	void EnterTheDoor();
};