#include "pch.h"
#include "CLayerUpdateBox.h"

#include <Engine\CTransform.h>
#include <Engine\CCollider3D.h>
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>
#include <Engine\CLayer.h>


CLayerUpdateBox::CLayerUpdateBox() 
	: CScript((UINT)SCRIPT_TYPE::LAYERUPDATEBOX)
	, m_layerNumber(1)
{
	SetName(L"LayerUpdateBox");
	// �ش� ������ �ʿ� ������Ʈ�� �����ִ� ������Ʈ�� ���� �ش� ��ũ��Ʈ�� ������Ʈ���� �־��ش�.
	// �� �� �÷��̾�� �ش� ������Ʈ�� �浹�ϰ� �Ǹ� �ش� ���̾� ��ȣ�� ���ؼ� ������Ʈ���� ������Ʈ ������ ���̴�.
	// �ش� ������Ʈ�� 0�� ���̾ �־��ְ� �� �������� ��ġ���� �ش�.
	// ���� ������Ʈ �� ���̾� üũ �� ������ �÷��̾� ��ũ��Ʈ���� ���� ��
}

CLayerUpdateBox::~CLayerUpdateBox()
{
}

void CLayerUpdateBox::SaveToScene(FILE* _file)
{
	SaveToFile(&m_layerNumber, _file);
}

void CLayerUpdateBox::LoadFromScene(FILE * _file)
{
	LoadFromFile(&m_layerNumber, _file);
}