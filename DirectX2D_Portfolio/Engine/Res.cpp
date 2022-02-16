#include "pch.h"
#include "Res.h"

Res::Res(RESOURCE_TYPE _type) :
	m_refCount(0),
	m_type(_type),
	m_bDefault(false)
{
}

Res::Res(RESOURCE_TYPE _type, bool _bDefault) :
	m_refCount(0),
	m_type(_type),
	m_bDefault(_bDefault)
{
}

Res::~Res()
{
}
