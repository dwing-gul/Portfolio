#pragma once

template <typename T>
class Singleton
{
private:
	static T* g_Inst;

public:
	static T* GetInst()
	{
		if (nullptr == g_Inst)
		{
			g_Inst = new T;
			atexit(&Singleton<T>::Destroy);
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
	Singleton() {}
	virtual ~Singleton() {}
};

template <typename T>
T* Singleton<T>::g_Inst = nullptr;