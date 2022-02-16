#pragma once
#include "CCamera.h"
#include "CLight2D.h"
#include "CLight3D.h"

class CStructuredBuffer;
class CTileMap;
class CMRT;
class CTexture;
class CGraphicsShader;
class CMaterial;

class CRenderManager :
	public CSingleton<CRenderManager>
{
	SINGLE(CRenderManager);
private:
	CMRT*					m_arrMRT[(UINT)MRT_TYPE::END];
	vector<CCamera*>		m_vecCam;
	CCamera*				m_toolCam;
	vector<CLight2D*>		m_vecLight2D;
	vector<CLight3D*>		m_vecLight3D;
	Ptr<CTexture>			m_postEffectTarget;

	CStructuredBuffer*		m_light2DBuffer;
	CStructuredBuffer*		m_light3DBuffer;

	CGraphicsShader*		m_lightShader[(UINT)LIGHT_TYPE::END];
	CMaterial*				m_lightMaterial[(UINT)LIGHT_TYPE::END];

	CGraphicsShader*		m_lightMergeShader;
	CMaterial*				m_lightMergeMaterial;

public:
	void init();
	void render();

private:
	void render_play();
	void render_tool();
	void render_shadowdepth();

public:
	void RegisterCamera(CCamera* _cam) { m_vecCam.push_back(_cam); }
	void RegisterLight2D(CLight2D* _light2D) { m_vecLight2D.push_back(_light2D); }
	UINT RegisterLight3D(CLight3D* _light3D) { m_vecLight3D.push_back(_light3D); return (UINT)(m_vecLight3D.size() - 1); }
	void SetToolCamera(CCamera* _toolCam) { m_toolCam = _toolCam; }
	void DeregisterCamera() { m_vecCam.clear(); }

	CCamera* GetToolCam() { return m_toolCam; }
	CCamera* GetMainCam();
	CMRT* GetMRT(MRT_TYPE _type) { return m_arrMRT[(UINT)_type]; }
	Ptr<CMaterial> GetLightMaterial(LIGHT_TYPE _type) { return m_lightMaterial[(UINT)_type]; }
	Ptr<CMaterial> GetLightMergeMaterial() { return m_lightMergeMaterial; }
	void CopyBackBuffer();

private:
	void render_init();
	void render_clear();
	void CreateLightBuffer();
	void CreateMRT();
	void ClearMRT();
	void CreateResource();
};

