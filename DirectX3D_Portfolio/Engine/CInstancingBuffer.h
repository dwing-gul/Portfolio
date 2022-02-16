#pragma once
#include "CStructuredBuffer.h"
#include "CTexture.h"

struct tInstancingData
{
	Matrix worldMatrix;
	Matrix matrixWV;
	Matrix matrixWVP;
	Matrix matrixVP;
	Matrix matrixV;
	int    rowIndex;
};

class CTexture;
class CStructuredBuffer;
class CCopyBoneMatrixShader;

class CInstancingBuffer
	: public CSingleton<CInstancingBuffer>
{
	SINGLE(CInstancingBuffer);

private:
	ComPtr<ID3D11Buffer>		m_instancingBuffer;
	UINT						m_maxCount;
	vector<tInstancingData>		m_vecData;

	//// anim3D ¿ëµµ
	vector<CStructuredBuffer*>	m_vecBoneMatrix;
	int							m_animInstancingCount;
	CStructuredBuffer*			m_boneBuffer;

	Ptr<CCopyBoneMatrixShader>	m_copyShader;

public:
	void init();
	void Clear()
	{
		m_vecData.clear();
		m_vecBoneMatrix.clear(); 
		m_animInstancingCount = 0; 
	}
	void AddInstancingData(tInstancingData& _tData) { m_vecData.push_back(_tData); }

	UINT GetInstanceCount() { return (UINT)m_vecData.size(); }
	ComPtr<ID3D11Buffer> GetBuffer() { return m_instancingBuffer; }
	void SetData();

	void AddInstancingBoneMatrix(CStructuredBuffer* _buffer);
	int GetAnimInstancingCount() { return m_animInstancingCount; };
	CStructuredBuffer* GetBoneBuffer() { return m_boneBuffer; }

private:
	void Resize(UINT _iCount);


};
