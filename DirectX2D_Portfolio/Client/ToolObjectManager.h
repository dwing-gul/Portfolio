#pragma once

class GameObjectEX;

class ToolObjectManager :
	public Singleton<ToolObjectManager>
{
	SINGLE(ToolObjectManager)
private :
	vector<GameObjectEX*> m_vectorToolObj;

public :
	void init();
	void progress();
};

