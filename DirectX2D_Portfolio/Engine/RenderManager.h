#pragma once

class Camera;
class Light2D;
class StructuredBuffer;

class RenderManager :
	public Singleton<RenderManager>
{
	SINGLE(RenderManager)
private :
	vector<Light2D*>  m_vectorLight2D;
	vector<Camera*>   m_vectorCam;
	Camera*			  m_toolCam;
	Ptr<Texture>	  m_postEffectTexture;
	StructuredBuffer* m_light2DBuffer;

public :
	void init();
	void render();

	void RegisterCamera(Camera* _cam) { m_vectorCam.push_back(_cam); }
	void RegisterLight2D(Light2D* _light) { m_vectorLight2D.push_back(_light); }
	void SetToolCamera(Camera* _cam) { m_toolCam = _cam; }
	void DeregisterCamera() { m_vectorCam.clear(); }

	Camera* GetToolCam() { return m_toolCam; }
	Camera* GetMainCam();
	void CopyBackBuffer();

private :
	void render_init();
	void render_play();
	void render_tool();
	void render_clear();
};

