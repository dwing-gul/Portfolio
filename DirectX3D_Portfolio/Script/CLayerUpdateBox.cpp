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
	// 해당 역할은 맵에 업데이트를 돌려주는 오브젝트를 만들어서 해당 스크립트를 오브젝트에게 넣어준다.
	// 그 후 플레이어와 해당 오브젝트가 충돌하게 되면 해당 레이어 번호를 통해서 오브젝트들을 업데이트 시켜줄 것이다.
	// 해당 오브젝트는 0번 레이어에 넣어주고 맵 지역마다 배치시켜 준다.
	// 현재 업데이트 될 레이어 체크 및 관리는 플레이어 스크립트에서 진행 중
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