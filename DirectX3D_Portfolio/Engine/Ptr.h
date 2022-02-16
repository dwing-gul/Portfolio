#pragma once

class CMaterial;
class CMesh;
class CTexture;
class CGraphicsShader;
class CComputeShader;
class CPrefab;

template <typename T>
class Ptr
{
private:
	T* m_res;

public:
	T* operator ->()
	{
		return m_res;
	}

	T* operator ->() const
	{
		return m_res;
	}

	T* Get()
	{
		return m_res;
	}

	T** GetAddressOf()
	{
		return &m_res;
	}

	Ptr& operator = (T* _res)
	{
		if (nullptr != m_res)
			m_res->SubRef();

		m_res = _res;

		if (nullptr != m_res)
			m_res->AddRef();

		return *this;
	}

	Ptr& operator = (const Ptr<T>& _ptr)
	{
		if (nullptr != m_res)
			m_res->SubRef();

		m_res = _ptr.m_res;

		if (nullptr != m_res)
			m_res->AddRef();

		return *this;
	}

	bool operator == (T* _res)
	{
		return m_res == _res ? true : false;
	}

	bool operator != (T* _res)
	{
		return !((*this) == _res);
	}

public:
	Ptr() :
		m_res(nullptr)
	{
	}

	Ptr(T* _res) :
		m_res(_res)
	{
		if (nullptr != m_res)
		{
			m_res->AddRef();
		}
	}

	Ptr(const Ptr<T>& _other) :
		m_res(_other.m_res)
	{
		if (nullptr != m_res)
		{
			m_res->AddRef();
		}
	}

	~Ptr()
	{
#ifdef _DEBUG
		if (nullptr != m_res && m_res->m_memCheck != 1)
		{
			int check = 0;
		}
#endif

		if (nullptr != m_res)
		{
			m_res->SubRef();
		}
	}
};

template <typename T>
bool operator == (void* _res, Ptr<T> _ptr)
{
	if (_res == _ptr.Get())
		return true;
	else
		return false;
}

template <typename T>
bool operator != (void* _res, Ptr<T> _ptr)
{
	return !(_res == _ptr);
}