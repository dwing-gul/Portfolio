#include "pch.h"
#include "CAnimator3D.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CAnimation3DShader.h"
#include "CTransform.h"

CAnimator3D::CAnimator3D() :
	CComponent(COMPONENT_TYPE::ANIMATOR3D),
	m_CurClip(0),
	m_CurTime(0.),
	m_FrameCount(30),
	m_BoneFinalMatrixBuffer(nullptr),
	m_bFinalMatrixUpdate(false),
	m_FrameIndex(0),
	m_NextFrameIndex(0),
	m_Ratio(0.f),
	m_vecBones(nullptr),
	m_vecClip(nullptr),
	m_bFinish(false),
	m_bRepeat(true),
	m_curActionCount(0),
	m_ActionCount(0),
	m_animSpeed(1.f),
	m_BoneMatrixBuffer(nullptr),
	m_BoneMaterial(nullptr),
	m_addAnimActionCount(0),
	m_addAnimActionMaxCount(0),
	m_addAnimCurTime(0),
	m_AddAnimFrameIndex(0),
	m_AddAnimNextFrameIndex(0),
	m_addPlayClip(0),
	m_addAnimRatio(0),
	m_bAddAnimFinish(false),
	m_bAddAnimRepeat(false),
	m_bAddPlayAnim(false),
	m_bonePos(0)

{
	m_BoneFinalMatrixBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin) :
	CComponent(COMPONENT_TYPE::ANIMATOR3D),
	m_CurClip(0),
	m_CurTime(0.f),
	m_FrameCount(30),
	m_BoneFinalMatrixBuffer(nullptr),
	m_bFinalMatrixUpdate(false),
	m_BoneMaterial(nullptr),
	m_FrameIndex(0),
	m_NextFrameIndex(0),
	m_Ratio(0.f),
	m_vecBones(nullptr),
	m_vecClip(nullptr),
	m_bFinish(false),
	m_bRepeat(false),
	m_curActionCount(0),
	m_ActionCount(0),
	m_animSpeed(1.f),
	m_BoneMatrixBuffer(nullptr),
	m_addAnimActionCount(0),
	m_addAnimActionMaxCount(0),
	m_addAnimCurTime(0),
	m_AddAnimFrameIndex(0),
	m_AddAnimNextFrameIndex(0),
	m_addPlayClip(0),
	m_addAnimRatio(0),
	m_bAddAnimFinish(false),
	m_bAddAnimRepeat(false),
	m_bAddPlayAnim(false),
	m_bonePos(_origin.m_bonePos)
{
	m_BoneFinalMatrixBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	ClearData();

	if (nullptr != m_BoneMaterial)
	{
		m_BoneMaterial = nullptr;
	}
	Safe_Delete_Ptr(m_BoneFinalMatrixBuffer);
}


void CAnimator3D::update()
{
}

void CAnimator3D::lateUpdate()
{
}

void CAnimator3D::finalUpdate()
{
	if (!m_bFinish)
	{
		m_CurTime = 0.f;
		// 현재 재생중인 Clip 의 시간을 진행한다.

		m_vecClipUpdateTime[m_CurClip] += DELTATIME * m_animSpeed;

		if (m_vecClipUpdateTime[m_CurClip] >= m_vecClip->at(m_CurClip).timeLength)
		{
			if (m_bRepeat && m_ActionCount == 0)
				m_vecClipUpdateTime[m_CurClip] = 0.f;
			else if (m_bRepeat && m_ActionCount != 0)
			{
				m_vecClipUpdateTime[m_CurClip] = 0.f;
				++m_curActionCount;

				if (m_curActionCount == m_ActionCount)
				{
					m_bRepeat = false;
					m_curActionCount = 0;
					m_ActionCount = 0;
				}
			}
			else
				m_vecClipUpdateTime[m_CurClip] -= DELTATIME * m_animSpeed;

			m_bFinish = m_bRepeat == true ? false : true;
		}

		m_CurTime = m_vecClip->at(m_CurClip).startTime + m_vecClipUpdateTime[m_CurClip];

		// 현재 애니메이션이 계속 같은 구간을 도는 것은 여기가 문제
		// 프레임 카운트가 고정되어 있었기 때문으로 각 애니메이션 클립의 프레임 수에 맞게 넣어줘야됨
		// 그리고 프레임 인덱스가 해당 애니메이션이 있는 위치로 가야되므로 지금의 프레임 인덱스 구하는 식에 
		// (현재 클립 인덱스 * 최대 프레임 갯수)를 곱해야 해당 애니메이션의 시작위치로 이동

		// 클립별 시작 위치를 계산하기 위한 변수
		int startPos = 0;

		for (int i = 0; i < m_CurClip; ++i)
		{
			startPos += m_vecClip->at(i).frameLength;
		}

		// 현재 프레임 인덱스 구하기
		double frameIndex = m_CurTime * (double)m_FrameCount;
		m_FrameIndex = (int)(frameIndex) + startPos;

		if (m_FrameIndex >= m_vecClip->at(m_CurClip).frameLength - 1 + startPos)
			m_NextFrameIndex = m_FrameIndex;	// 끝이면 현재 인덱스를 유지
		else
			m_NextFrameIndex = m_FrameIndex + 1;

		// 프레임간의 시간에 따른 비율을 구해준다.
		m_Ratio = (float)(frameIndex - (double)(m_FrameIndex - startPos));

		// 컴퓨트 쉐이더 연산여부
		m_bFinalMatrixUpdate = false;
	}

	//if (m_bAddPlayAnim && !m_bAddAnimFinish)
	//{
	//	m_addAnimCurTime = 0.f;
	//	// 현재 재생중인 Clip 의 시간을 진행한다.

	//	m_vecClipUpdateTime[m_addPlayClip] += DELTATIME * m_animSpeed;

	//	if (m_vecClipUpdateTime[m_addPlayClip] >= m_vecClip->at(m_addPlayClip).timeLength)
	//	{
	//		if (m_bAddAnimRepeat && m_addAnimActionCount == 0)
	//			m_vecClipUpdateTime[m_addPlayClip] = 0.f;
	//		else if (m_bAddAnimRepeat && m_addPlayClip != 0)
	//		{
	//			m_vecClipUpdateTime[m_addPlayClip] = 0.f;
	//			++m_addAnimActionCount;

	//			if (m_addAnimActionCount == m_addAnimActionMaxCount)
	//			{
	//				m_bAddAnimRepeat = false;
	//				m_addAnimActionCount = 0;
	//				m_addAnimActionMaxCount = 0;
	//			}
	//		}
	//		else
	//			m_vecClipUpdateTime[m_addPlayClip] -= DELTATIME * m_animSpeed;

	//		m_bAddAnimFinish = m_bAddAnimRepeat == true ? false : true;
	//	}

	//	m_addAnimCurTime = m_vecClip->at(m_addPlayClip).startTime + m_vecClipUpdateTime[m_addPlayClip];

	//	// 현재 애니메이션이 계속 같은 구간을 도는 것은 여기가 문제
	//	// 프레임 카운트가 고정되어 있었기 때문으로 각 애니메이션 클립의 프레임 수에 맞게 넣어줘야됨
	//	// 그리고 프레임 인덱스가 해당 애니메이션이 있는 위치로 가야되므로 지금의 프레임 인덱스 구하는 식에 
	//	// (현재 클립 인덱스 * 최대 프레임 갯수)를 곱해야 해당 애니메이션의 시작위치로 이동

	//	// 클립별 시작 위치를 계산하기 위한 변수
	//	int startPos = 0;

	//	for (int i = 0; i < m_addPlayClip; ++i)
	//	{
	//		startPos += m_vecClip->at(i).frameLength;
	//	}

	//	// 현재 프레임 인덱스 구하기
	//	double frameIndex = m_addAnimCurTime * (double)m_FrameCount;
	//	m_AddAnimFrameIndex = (int)(frameIndex)+startPos;

	//	if (m_FrameIndex >= m_vecClip->at(m_CurClip).frameLength - 1 + startPos)
	//		m_AddAnimNextFrameIndex = m_AddAnimFrameIndex;	// 끝이면 현재 인덱스를 유지
	//	else
	//		m_AddAnimFrameIndex = m_AddAnimFrameIndex + 1;

	//	// 프레임간의 시간에 따른 비율을 구해준다.
	//	m_Ratio = (float)(frameIndex - (double)(m_AddAnimFrameIndex - startPos));

	//	// 컴퓨트 쉐이더 연산여부
	//	m_bFinalMatrixUpdate = false;
	//}
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_vecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_vecClip->size());


	static float fTime = 0.f;
	fTime += 1.f;

	for (size_t i = 0; i < m_vecClipUpdateTime.size(); ++i)
	{
		m_vecClipUpdateTime[i] = (float)_vecAnimClip->at(i).timeLength;
	}
}

void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatrixUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* UpdateShader = (CAnimation3DShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateShader").Get();

		// Bone Data
		Ptr<CMesh> mesh = MeshRender()->GetMesh();
		check_mesh(mesh);

		UpdateShader->SetFrameDataBuffer(mesh->GetBoneFrameDataBuffer());
		UpdateShader->SetOffsetMatrixBuffer(mesh->GetBoneOffsetBuffer());
		UpdateShader->SetOutputBuffer(m_BoneFinalMatrixBuffer);
		
		if (nullptr != m_BoneMatrixBuffer)
		{
			UpdateShader->SetBoneMatrixBuffer(m_BoneMatrixBuffer);
			UpdateShader->SetBoneIndex(m_bonePos);
		}


		UINT boneCount = (UINT)m_vecBones->size();
		UpdateShader->SetBoneCount(boneCount);
		UpdateShader->SetFrameIndex(m_FrameIndex);
		UpdateShader->SetNextFrameIdx(m_NextFrameIndex);
		UpdateShader->SetFrameRatio(m_Ratio);
		
		//if (m_bAddPlayAnim)
		//{
		//	UpdateShader->SetAddAnimClip(m_bAddPlayAnim);
		//	UpdateShader->SetAddFrameIndex(m_AddAnimFrameIndex);
		//	UpdateShader->SetAddNextFrameIdx(m_AddAnimNextFrameIndex);
		//	UpdateShader->SetAddFrameRatio(m_addAnimRatio);
		//}

		// 업데이트 쉐이더 실행
		UpdateShader->Excute();

		m_bFinalMatrixUpdate = true;
	}

	// t12 레지스터에 최종행렬 데이터(구조버퍼) 바인딩	
	m_BoneFinalMatrixBuffer->UpdateData(12);

	if (nullptr != m_BoneMatrixBuffer && nullptr != m_BoneMaterial && !Object()->IsDead())
	{
		m_HandboneMatrix = Transform()->GetWorldMatrix();

		m_BoneMatrixBuffer->UpdateData(14);
		//본의 월드 행렬 값
		m_BoneMaterial->SetData(SHADER_PARAM::MATRIX_1, &m_HandboneMatrix);
	}
	
}

void CAnimator3D::ClearData()
{
	m_BoneFinalMatrixBuffer->Clear();

	UINT materialCount = (UINT)MeshRender()->GetMaterialCount();
	Ptr<CMaterial> material = nullptr;
	for (UINT i = 0; i < materialCount; ++i)
	{
		material = MeshRender()->GetSharedMaterial(i);
		if (nullptr == material)
			continue;
		material->SetAnim3D(false); // Animation Mesh 알리기
		material->SetBoneCount(0);
	}
}

void CAnimator3D::Play(const wstring& _name, bool _bRepeat ,float _animSpeed, int _actionCount)
{
	// 이전에 동작하던 시간이 남아있는 경우를 대비하여 리셋
	if (FindClip(_name))
	{
		SetRepeat(_bRepeat , _actionCount);
		Reset();
		m_animSpeed = _animSpeed;
	}
	else
		return;
}

int CAnimator3D::FindClipIndex(const wstring& _name)
{
	for (int i = 0; i < m_vecClip->size(); ++i)
	{
		if (wstring(m_vecClip->at(i).animName.end() - _name.size(), m_vecClip->at(i).animName.end()) == _name)
		{
			return i;
		}
	}

	return -1;
}

void CAnimator3D::AddAnimPlay(const wstring& _name, bool _bRepeat, float _animSpeed, int _actionCount)
{
	m_bAddPlayAnim = true;
	m_addPlayClip = FindClipIndex(_name);
	if (-1 != m_addPlayClip)
	{
		SetAddAnimRepeat(_bRepeat, _actionCount);
		AddReset();
		//m_animSpeed = _animSpeed;
	}
	else
		return;
}

bool CAnimator3D::FindClip(const wstring& _name)
{
	for (int i = 0; i < m_vecClip->size(); ++i)
	{
		if (m_vecClip->at(i).animName.size() >= _name.size() 
			&& wstring(m_vecClip->at(i).animName.end() - _name.size(), m_vecClip->at(i).animName.end()) == _name)
		{
			m_CurClip = i;
			return true;
		}
	}

	return false;
}

wstring CAnimator3D::GetFindClipName(const wstring& _name)
{
	for (int i = 0; i < m_vecClip->size(); ++i)
	{
		if (m_vecClip->at(i).animName.size() >= _name.size()
			&& wstring(m_vecClip->at(i).animName.end() - _name.size(), m_vecClip->at(i).animName.end()) == _name)
		{
			return m_vecClip->at(i).animName;
		}
	}

	return L"";
}

bool CAnimator3D::CheckCurClip(const wstring& _name)
{
	if (m_vecClip->at(m_CurClip).animName == _name)
		return true;
	else
		return false;
}

void CAnimator3D::SetFrameIndexMotion(int _frame)
{
	m_FrameIndex = _frame;
	m_NextFrameIndex = m_FrameIndex + 1;
	m_bFinish = true;
	m_bFinalMatrixUpdate = false;
	m_Ratio = 0.f;
}

void CAnimator3D::SetRatio(float _ratio)
{
	m_Ratio = _ratio;
	m_bFinish = true;
	m_bFinalMatrixUpdate = false;
}

void CAnimator3D::check_mesh(Ptr<CMesh> _mesh)
{
	UINT iBoneCount = _mesh->GetBoneCount();
	if (m_BoneFinalMatrixBuffer->GetElementCount() != iBoneCount)
	{
		m_BoneFinalMatrixBuffer->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(Matrix), iBoneCount, nullptr);
	}
}

void CAnimator3D::SaveToScene(FILE* _file)
{
}

void CAnimator3D::LoadFromScene(FILE* _file)
{
}