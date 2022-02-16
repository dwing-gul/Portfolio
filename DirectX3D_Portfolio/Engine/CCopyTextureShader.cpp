#include "pch.h"
#include "CCopyTextureShader.h"
#include "CConstBuffer.h"

CCopyTextureShader::CCopyTextureShader() :
	CComputeShader(32, 32, 1)
{
}

CCopyTextureShader::~CCopyTextureShader()
{
}

void CCopyTextureShader::SetSrcTexture(Ptr<CTexture> _texture)
{
	m_srcTex = _texture;
	m_info.vec2Arr[0] = Vec2(m_srcTex->Width(), m_srcTex->Height());
}

void CCopyTextureShader::SetDestTexture(Ptr<CTexture> _texture)
{
	m_destTex = _texture;
	m_info.vec2Arr[1] = Vec2(m_destTex->Width(), m_destTex->Height());
}

void CCopyTextureShader::UpdateData()
{
	g_materialBuffer->SetData(&m_info);
	g_materialBuffer->UpdateData(SHADER_STAGE::Compute);

	m_srcTex->UpdateData(SHADER_STAGE::Compute, 0);
	m_destTex->UpdateRWData(0);
}

void CCopyTextureShader::Clear()
{
	m_srcTex = nullptr;
	m_destTex = nullptr;

	CTexture::Clear(0);
	CTexture::ClearRW(0);
}

void CCopyTextureShader::Excute()
{
	Vec2 resolution = Vec2(m_destTex->Width(), m_destTex->Height());

	UINT groupX = ((UINT)resolution.x / GetThreadX()) + 1;
	UINT groupY = ((UINT)resolution.y / GetThreadY()) + 1;

	Dispatch(groupX, groupY, 1);
}
