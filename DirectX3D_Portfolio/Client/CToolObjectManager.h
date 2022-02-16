#pragma once

class CGameObjectEX;
class CMaterial;
class CGraphicsShader;

class CToolObjectManager :
	public CSingleton<CToolObjectManager>
{
	SINGLE(CToolObjectManager)
private :
	vector<CGameObjectEX*> m_vecToolObj;

	CMaterial* m_gridMaterial;
	CGraphicsShader* m_gridShader;

public :
	void init();
	void progress();
};

