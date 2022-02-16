#pragma once
#include "CRes.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial :
	public CRes
{
private:
	Ptr<CGraphicsShader> m_shader;
	tMaterialParam m_param;
	Ptr<CTexture> m_arrTexture[TEXTURE_COUNT];

public:
	void SetShader(Ptr<CGraphicsShader> _shader) { m_shader = _shader; }
	void SetData(SHADER_PARAM _param, void* _data);

	Ptr<CGraphicsShader> GetShader() { return m_shader; }
	void GetData(SHADER_PARAM _param, void* _out);

	void SetMaterialCoefficient(Vec4 _diffuse, Vec4 _specular, Vec4 _ambient, Vec4 _emissive)
	{
		m_param.material.diffuse = _diffuse;
		m_param.material.specular = _specular;
		m_param.material.ambient = _ambient;
		m_param.material.emissive = _emissive;
	}

	void SetAnim3D(bool _bTrue) { m_param.arrAnimData[0] = (int)_bTrue; }
	void SetBoneCount(int _boneCount) { m_param.arrAnimData[1] = _boneCount; }

	virtual void UpdateData();
	void UpdateData_Inst();
	virtual void Save(const wstring& _relativePath);
	virtual int Load(const wstring& _filePath);

	static void Clear();

	virtual CMaterial* Clone();

public:
	CMaterial();
	CMaterial(bool _bDefault);
	~CMaterial();
};