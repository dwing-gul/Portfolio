#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;

class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*		m_vecBones;
	const vector<tMTAnimClip>*	m_vecClip;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMatrix;		// �ؽ��Ŀ� ������ ���� �������
	int							m_FrameCount;				// �ʴ� �׸� �ִϸ��̼� ������ ��
	double						m_CurTime;
	int							m_CurClip;					// Ŭ�� �ε���	
	int							m_curActionCount;
	int							m_ActionCount;

	int							m_FrameIndex;				// Ŭ���� ���� ������
	int							m_NextFrameIndex;			// Ŭ���� ���� ������
	float						m_Ratio;					// ������ ���� ����

	CStructuredBuffer*			m_BoneFinalMatrixBuffer;	// Ư�� �������� ���� ���
	bool						m_bFinalMatrixUpdate;		// ������� ���� ���࿩��
	bool						m_bFinish;
	bool						m_bRepeat;
	float						m_animSpeed;

	int							m_bonePos;

	CStructuredBuffer*			m_BoneMatrixBuffer;
	CMaterial*					m_BoneMaterial;
	// �߰��� �ʿ��� �ִϸ��̼� �ɼ�
	int							m_addAnimActionCount;
	int							m_addAnimActionMaxCount;
	double						m_addAnimCurTime;
	int							m_AddAnimFrameIndex;				// Ŭ���� ���� ������
	int							m_AddAnimNextFrameIndex;			// Ŭ���� ���� ������
	
	int							m_addPlayClip;
	float						m_addAnimRatio;
	bool						m_bAddAnimFinish;
	bool						m_bAddAnimRepeat;
	bool						m_bAddPlayAnim;
	Matrix						m_boneMatrix;
	Matrix						m_HandboneMatrix;
	Matrix						m_HandAnimMatrix;
	Matrix						m_HandOffSetMatrix;
public:
	void SetBones(const vector<tMTBone>* _vecBones) { m_vecBones = _vecBones; m_vecFinalBoneMatrix.resize(m_vecBones->size()); }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);

	void UpdateData();
	void SetClipTime(int _clipIndex, float _time) { m_vecClipUpdateTime[_clipIndex] = _time; }

	void SetCurClipIndex(int clipIndex, bool _repeat = true , int _actionCount = 0)
	{
		m_CurClip = clipIndex; m_vecClipUpdateTime[m_CurClip] = 0.f; m_bFinish = false; SetRepeat(_repeat, _actionCount);
	}
	int GetCurClipIndex() { return m_CurClip; }
	int GetClipSize() { return (int)m_vecClip->size(); }

	CStructuredBuffer* GetFinalBoneMat() { return m_BoneFinalMatrixBuffer; }
	UINT GetBoneCount() { return (UINT)m_vecBones->size(); }
	void ClearData();

	bool FindClip(const wstring& _name);
	wstring GetFindClipName(const wstring& _name);
	bool CheckCurClip(const wstring& _name);

	int FindClipIndex(const wstring& _name);
	void SetRepeat(bool _repeat, int _actionCount = 0) { m_bRepeat = _repeat; m_curActionCount = 0; m_ActionCount = _actionCount; if (0 != m_ActionCount) m_bRepeat = true; }
	void SetAddAnimRepeat(bool _repeat, int _actionCount = 0) { m_bAddAnimRepeat = _repeat; m_addAnimActionCount = 0; m_addAnimActionMaxCount = _actionCount;}
	void Play(const wstring& _name, bool _bRepeat, float _animSpeed = 1.f, int _actionCount = 0);

	void AddAnimPlay(const wstring& _name, bool _bRepeat, float _animSpeed = 1.f, int _actionCount = 0);

	bool IsAnimFinish() { return m_bFinish; }
	void Reset()
	{
		m_vecClipUpdateTime[m_CurClip] = 0.f;
		m_bFinish = false;
	}

	void AddReset()
	{
		m_vecClipUpdateTime[m_addPlayClip] = 0.f;
		m_bAddAnimFinish = false;
	}
	bool GetRepeat() { return m_bRepeat; }

	void SetFrameIndexMotion(int _frame);
	int GetFrameSize() { return (int)m_vecBones->at(2).vecKeyFrame.size(); }
	UINT GetCurFrameIndex() { return UINT(m_CurTime / 0.033333333333); }
	int GetCurRealFrameIndex() { return m_FrameIndex; }
	int GetCurAnimMaxFrame() { return m_vecClip->at(m_CurClip).endFrame; }

	void SetAnimSpeed(float _speed) { m_animSpeed = _speed; }
	float GetAnimSpeed() { return m_animSpeed; }

	wstring	GetCurAnimName() { return m_vecClip->at(m_CurClip).animName; }

	double GetCurTime() { return m_CurTime; }

	float GetRatio() { return m_Ratio; }
	void SetRatio(float _ratio);

public:
	void SetMatrixBuffer(CStructuredBuffer* _buffer) { m_BoneMatrixBuffer = _buffer; }
	void SetBonePos(int _boneIndex) { m_bonePos = _boneIndex; }
	void SetBoneMaterial(CMaterial* _mtrl) { m_BoneMaterial = _mtrl; }

private:
	void check_mesh(Ptr<CMesh> _mesh);

public:
	virtual void start() {};
	virtual void update();
	virtual void lateUpdate();
	virtual void finalUpdate();

public:
	virtual void SaveToScene(FILE* _file);
	virtual void LoadFromScene(FILE* _file);
	
	CLONE(CAnimator3D);

public:
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	virtual ~CAnimator3D();
};




