#include "pch.h"
#include "CRes.h"

CRes::CRes(RESOURCE_TYPE _type) :
	m_type(_type),
	m_refCount(0),
	m_bDefault(false)
{
}

CRes::CRes(RESOURCE_TYPE _type, bool _bDefault) :
	m_type(_type),
	m_refCount(0),
	m_bDefault(_bDefault)
{
}

CRes::~CRes()
{
}
