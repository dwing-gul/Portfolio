#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CAnimation3DShader :
	public CComputeShader
{
private:
	tMaterialParam			m_info;						// g_int_0 : BonCount, g_int_1 : Frame Index
	CStructuredBuffer*		m_FrameDataBuffer;			// t13
	CStructuredBuffer*		m_OffsetMatrixBuffer;			// t14 
	CStructuredBuffer*		m_OutputBuffer;				// u0

	CStructuredBuffer*		m_BoneMatrixBuffer;			// u1

public:
	void SetBoneCount(int _iBoneCount) { m_info.intArr[0] = _iBoneCount; }
	void SetFrameIndex(int _iFrameIdx) { m_info.intArr[1] = _iFrameIdx; }
	void SetNextFrameIdx(int _iFrameIdx) { m_info.intArr[2] = _iFrameIdx; }

	void SetAddAnimClip(bool _b) { m_info.intArr[3] = (int)_b; }

	void SetAddFrameIndex(int _iFrameIdx) { m_info.vec2Arr[0].x = (float)_iFrameIdx; }
	void SetAddNextFrameIdx(int _iFrameIdx) { m_info.vec2Arr[0].y = (float)_iFrameIdx; }

	void SetFrameRatio(float _fFrameRatio) { m_info.floatArr[0] = _fFrameRatio; }
	void SetAddFrameRatio(float _fFrameRatio) { m_info.floatArr[1] = _fFrameRatio; }

	void SetBoneIndex(int _frameIdx) { m_info.intArr[3] = (int)_frameIdx; }

	void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_FrameDataBuffer = _buffer; }
	void SetOffsetMatrixBuffer(CStructuredBuffer* _buffer) { m_OffsetMatrixBuffer = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_OutputBuffer = _buffer; }
	void SetBoneMatrixBuffer(CStructuredBuffer* _buffer) { m_BoneMatrixBuffer = _buffer; }

public:
	virtual void UpdateData();
	virtual void Clear();
	virtual void Excute();

public:
	CAnimation3DShader();
	virtual ~CAnimation3DShader();
};


