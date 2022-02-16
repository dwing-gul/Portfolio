#pragma once

template<typename T>
class CSingleton
{
private:
	static T* g_Inst;

public:
	static T* GetInst()
	{
		if (nullptr == g_Inst)
		{
			g_Inst = new T;
			atexit(&CSingleton<T>::Destroy);
		}

		return g_Inst;
	}

private:
	static void Destroy()
	{
		if (nullptr != g_Inst)
		{
			delete g_Inst;
			g_Inst = nullptr;
		}
	}

protected:
	CSingleton() {}
	virtual ~CSingleton() {}
};

template<typename T>
T* CSingleton<T>::g_Inst = nullptr;