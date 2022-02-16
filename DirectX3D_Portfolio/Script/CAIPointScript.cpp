#include "pch.h"
#include "CAIPointScript.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"

CAIPointScript::CAIPointScript() :
	CScript((UINT)SCRIPT_TYPE::AIPOINTSCRIPT),
	m_owner(nullptr),
	m_bArrive(false),
	m_bNext(false),
	m_curPointID(0),
	m_prevPointID(0)
{
}

CAIPointScript::~CAIPointScript()
{
}

void CAIPointScript::AddPointID()
{
	m_prevPointID = m_curPointID;
	++m_curPointID;

	if ((int)m_vecPoint.size() <= m_curPointID)
		m_curPointID = 0;
}

void CAIPointScript::SetMovePos()
{
	AddPointID();

	m_bNext = true;
}

void CAIPointScript::SetPointAction()
{
	// ������ ��� next �ʱ�ȭ
	m_bNext = false;

	// �÷��̾ �ش� point�� �������� ���
	switch (m_vecPoint[m_curPointID].m_pointType)
	{
	case POINT_TYPE::MOVE:
	{
		SetMovePos();
	}
	break;
	case POINT_TYPE::LOOTING:
	{
		const vector<CGameObject*>& vecItemSpot = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::INTERACTION, true);
		float distance = 500.f;

		CGameObject* lootingTarget = nullptr;
		for (int i = 0; i < vecItemSpot.size(); ++i)
		{
			float targetDistance = Vec3::Distance(m_vecPoint[m_curPointID].m_pointPos.XZ(), vecItemSpot[i]->Transform()->GetWorldPos().XZ());
			if (distance > targetDistance)
			{
				lootingTarget = vecItemSpot[i];
				distance = targetDistance;
			}
		}
		((CPlayerScript*)m_owner->GetScripts()[0])->SetAILootTarget(lootingTarget);
	}
	break;
	case POINT_TYPE::HUNTING:
	{
		const vector<CGameObject*>& vecMonster = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);

		Vec3 pos = m_vecPoint[m_curPointID].m_pointPos.XZ();
		for (int i = 0; i < (int)vecMonster.size(); ++i)
		{
			if (!vecMonster[i]->IsOn() || ((CMonsterScript*)vecMonster[i]->GetScripts()[0])->IsCharacterDead())
				continue;

			//ó�� ���� ����� ���͸� ���̰� �� ������ ����� ���͸� ���̷� �����ϱ� ������ �ִ� ������ ��� ����
			if (500.f >= Vec3::Distance(pos, vecMonster[i]->Transform()->GetLocalPos().XZ()))
			{
				((CPlayerScript*)m_owner->GetScripts()[0])->FindMonster(vecMonster[i]);
			}
		}
	}
	break;
	}
}

int CAIPointScript::GetFindPointID()
{
	// ó�� �̵��ؾ��� ID�� ã�� ���, Ȥ�� �÷��̾�� ������ ���� ���, �÷��̾� ���ǰ� Ǯ���� ���
	if (m_vecPoint.empty())
		return -1;

	float distance = 1000000.f;
	int id = -1;
	for (int i = 0; i < (int)m_vecPoint.size(); ++i)
	{
		float objDistance = Vec3::Distance(m_owner->Transform()->GetLocalPos().XZ(), m_vecPoint[i].m_pointPos.XZ());

		if (distance > objDistance)
		{
			distance = objDistance;
			id = m_vecPoint[i].m_pointID;
		}
	}

	m_curPointID = id;

	return id;
}

void CAIPointScript::SaveToScene(FILE* _file)
{
	UINT vecSize = (UINT)m_vecPoint.size();
	SaveToFile(&vecSize, _file);

	for (size_t i = 0; i < m_vecPoint.size(); ++i)
	{
		SaveToFile(&m_vecPoint[i], _file);
	}
}

void CAIPointScript::LoadFromScene(FILE* _file)
{
	int vecSize = 0;
	LoadFromFile(&vecSize, _file);

	for (UINT i = 0; i < (UINT)vecSize; ++i)
	{
		tPointType pointType = {};
		LoadFromFile(&pointType, _file);
		m_vecPoint.push_back(pointType);
	}
}

//--------------------------------------------------------------
// ���� : ai�� �̵��� �̵� ����Ʈ ���� -> ������ ���� Ÿ���� �޾� �� ���� ���� ������ �ο�
// Ÿ�� : �̵�, ����, ���
// 
// - ���� : ai�� �������� �����ϴ� ���� �� �� ai�� �ش� �������� ���� ����� ������ ���� ������
//			Ž���Ͽ� �������� ȹ���ϰ� ������ ������ ��ħ -> ai�� �ʿ��� �����۸��� ���� �� �ֵ��� �ؾߵ�
// 
// - ��� : ai�� �߻������� ��� ����ġ�� ��� ����. �ش� ���� ��ó�� �����ϸ� �ֺ��� �߻������� ��´�. 
//			���� ���� �߻��������Լ� �������� �򵵷� �ؾߵ�
// 
// - ���� ����
// 1. ai�� ���ͷ� �ش� ����Ʈ�� ������ �ְ� ���������� ����Ʈ�� �̵�
// 2. ai�� ����Ʈ ���޽� ����Ʈ�� Ÿ�Կ� �°� ����Ʈ�� ���� ����
//		2-1. ���� ����Ʈ�� ��� �ش� ����Ʈ ���� �ÿ� �ش� ����Ʈ���� ���� ����� ������ ���� Ž��
//			 ������ ���� �� ������ ���ձ��� �Ϸ� -> ���ʿ� ���ϴ� ������ ���� ���� �־�� �Ѵ�.
//			 -> ����Ʈ���� ����� ������ ������ 2�� �̻��� ��� �� �߿��� ���� ����� 1������ Ž�� or ���ͷ� �޾Ƽ� ���� ������ ������ Ž�� 
//			 -> �ٸ� �� �� ��� ������ �ϴ��� ����Ʈ�� ������ ���� �����̿� �־���ϸ� ������ ai�� ��ü������ Ž���ϴ� ���� ����Ʈ�� Ž���ϵ��� �ؾߵ�
//			 -> �׸��� ai�� ������ ���� ��ȣ�ۿ� ������ ���̴� ���� ������
//		2-2. ��� ����Ʈ�� ��� �ش� ����Ʈ���� ���� �Ÿ���ŭ ��������� ĳ���� ���� �ֺ��� ���� Ž��.
//			 -> ���� ��� ����. �ش� ���� ������ �ٸ� ���� Ž�� �ݺ�.
//			 -> �ش� ����Ʈ�� ��� �߻������� �׾��ٸ� �����ִ� ��ü�� ���Ͽ� ���� ����
//			 -> ���� �Ϸ� �� �ٸ� ����Ʈ�� �̵�
// 3. ���Ϳ� ��ϵ� ������ ����Ʈ���� ���� �Ͽ��ٸ� �ٽ� �Ųٷ� ���鼭 ����
// 4. �߰��� ������ �����Ͽ� �÷��̾�� ������ �ϴ� ���
//		-> �÷��̾�� ���� �� ���� ��ġ���� ���� ����� ����Ʈ Ž�� -> �ش� ����Ʈ�� �̵� �� �ٽ� 1�� ���� ����
// 5. �߰��� �÷��̾ ai�� �ٲ�� ���
//		-> �� ��ġ���� ���� ����� ����Ʈ Ž�� -> �ش� ����Ʈ�� �̵� �� 1�� ���� ����.
// 
// - �� ����Ʈ���� �Ϸῡ ���� bool ���� �����Ͽ� �ش� ����Ʈ���� ������ ��� ��ģ���� �˷��� �� ���� ����Ʈ�� �����ϵ��� ����.
// - �ٸ� ����Ʈ ��� �� �� �� ����Ʈ�� bool ���� �ʱ�ȭ
// 
// - owner�� ��ġ�� ����Ʈ ������ �Ÿ��� �����Ͽ� ���� �Ÿ�(�̰� Ÿ�Ժ��� ������ߵ�)������ ������ ���� Ȯ�� �� �ش� ����Ʈ������ ���� ����
// - ���� ����Ʈ�� �Ѿ ���� next ���� bool ���� �����Ͽ� �ش� �κ��� true�̸� ��� ���������� ���� ���� ���� ����
//--------------------------------------------------------------