#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CCopyBoneMatrixShader :
    public CComputeShader
{
private:
	// g_int_0 : BonCount
	// g_int_1 : RowIndex	
	CStructuredBuffer* m_sourceBuffer;
	CStructuredBuffer* m_destBuffer;

public:
	void SetBoneCount(UINT _boneCount) { m_info.intArr[0] = _boneCount; }
	void SetRowIndex(UINT _rowIndex) { m_info.intArr[1] = _rowIndex; }
	void SetSourceBuffer(CStructuredBuffer* _buffer) { m_sourceBuffer = _buffer; }
	void SetDestBuffer(CStructuredBuffer* _buffer) { m_destBuffer = _buffer; }

public:
	virtual void UpdateData();
	virtual void Clear();
	virtual void Excute();

public:
	CCopyBoneMatrixShader();
	virtual ~CCopyBoneMatrixShader();
};

