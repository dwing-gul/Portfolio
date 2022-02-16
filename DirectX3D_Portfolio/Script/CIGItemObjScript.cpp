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
	//�ش� �Լ��� �ڽ��� ���׸����� Ŭ�� ���׸���� ����, ��� ���ۿ� �� ����
	//Bonepos �Լ��� ���� �ڽ��� �θ𿡰� ��ǻƮ ���̴����� �޾ƿ� �� ��ġ�� �־���
	m_boneIndex = _EquipBoneID;

	//�ش� ����ȭ ���۸� ���⼭ �־��ִ� ���� �ƴ϶� �÷��̾ ���� ������ �ֱ� ������ �÷��̾ �־��ְ� ����� ��
	m_ownerObj->Animator3D()->SetBonePos(m_boneIndex);
	m_sharedMaterial = MeshRender()->GetSharedMaterial();
	m_cloneMaterial = MeshRender()->GetCloneMaterial();

	MeshRender()->SetMaterial(m_cloneMaterial);
	//�ش� �Լ����� �ڽ��� ����ϴ� �� index�� �־���
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
		//�ڽ��� off �Ǿ���� �ε����� ��� off ������
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

