#include "pch.h"
#include "CCameraEX.h"

CCameraEX::CCameraEX()
{
}

CCameraEX::~CCameraEX()
{
}

void CCameraEX::finalUpdate()
{
	CalViewMatrix();
	CalProjectionMatrix();
	CalRay();

	GetFrustum().finalUpdate();

	g_transform.viewMatrix = GetViewMatrix();
	g_transform.projectionMatrix = GetProjectionMatrix();
}
