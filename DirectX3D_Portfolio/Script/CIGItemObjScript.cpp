#include "pch.h"
#include "CIGItemObjScript.h"

#include <Engine\CAnimator3D.h>
#include <Engine\CStructuredBuffer.h>

CIGItemObjScript::CIGItemObjScript():
	CScript((UINT)SCRIPT_TYPE::IGITEMOBJSCRIPT),
	m_ownerObj(nullptr),
	m_bEquipItem(false),
	m_bAppearMotion(false),
	m_bCraftMotion(false),
	m_curFrame(-1),
	m_objOffFrame(-1),
	m_boneIndex(0)
{
	SetName(L"IGItemObjScript");
}

CIGItemObjScript::CIGItemObjScript(const CIGItemObjScript& _origin) :
	CScript((UINT)SCRIPT_TYPE::IGITEMOBJSCRIPT),
	m_ownerObj(nullptr),
	m_bEquipItem(false),
	m_bAppearMotion(false),
	m_bCraftMotion(false),
	m_curFrame(-1),
	m_objOffFrame(-1),
	m_boneIndex(_origin.m_boneIndex)
{
	SetName(L"IGItemObjScript");
}

CIGItemObjScript::~CIGItemObjScript()
{
	

}

void CIGItemObjScript::start()
{
	if (m_ownerObj == nullptr && nullptr != Object()->GetParent())
		m_ownerObj = Object()->GetParent();
}

void CIGItemObjScript::update()
{
	
}

void CIGItemObjScript::lateUpdate()
{
}

void CIGItemObjScript::SetEquipOption(bool _bOption, int _EquipBoneID)
{
	//해당 함수는 자신의 메테리얼을 클론 메테리얼로 변경, 상수 버퍼에 값 세팅
	//Bonepos 함수를 통해 자신의 부모에게 컴퓨트 쉐이더에서 받아올 뼈 위치를 넣어줌
	m_boneIndex = _EquipBoneID;

	//해당 구조화 버퍼를 여기서 넣어주는 것이 아니라 플레이어가 직접 가지고 있기 때문에 플레이어가 넣어주게 해줘야 함
	m_ownerObj->Animator3D()->SetBonePos(m_boneIndex);
	m_sharedMaterial = MeshRender()->GetSharedMaterial();
	m_cloneMaterial = MeshRender()->GetCloneMaterial();

	MeshRender()->SetMaterial(m_cloneMaterial);
	//해당 함수에서 자신이 사용하는 본 index를 넣어줌
	m_cloneMaterial->SetData(SHADER_PARAM::INT_0, &m_boneIndex);
}

void CIGItemObjScript::SetAppearMotion(bool _bMotion, float _speed)
{
	m_bCraftMotion = false;
	m_bAppearMotion = true;
	Animator3D()->Play(L"appear", false, _speed);
}

void CIGItemObjScript::SetCurFramePos()
{
	if (m_ownerObj->Animator3D()->GetCurFrameIndex() != m_curFrame)
	{
		//자신이 off 되어야할 인덱스인 경우 off 시켜줌
		if (-1 != m_objOffFrame && m_ownerObj->Animator3D()->GetCurFrameIndex() == m_objOffFrame)
		{
			Object()->SetObjOff();

			return;
		}
	}
}

void CIGItemObjScript::SetLocalMatrixUpdate(bool _b)
{
	Transform()->SetLocalMatrixUpdate(_b);
}

void CIGItemObjScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_bAppearMotion, _file);
	SaveToFile(&m_bCraftMotion, _file);
	SaveToFile(&m_objOffFrame, _file);
}

void CIGItemObjScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_bAppearMotion, _file);
	LoadFromFile(&m_bCraftMotion, _file);
	LoadFromFile(&m_objOffFrame, _file);
}

