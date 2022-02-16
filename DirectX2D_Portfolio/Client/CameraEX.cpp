#include "pch.h"
#include "CameraEX.h"

CameraEX::CameraEX()
{
}

CameraEX::~CameraEX()
{
}

void CameraEX::finalUpdate()
{
	CalViewMatrix();
	CalProjectionMatrix();

	g_transform.viewMatrix = GetViewMatrix();
	g_transform.projectionMatrix = GetProjectionMatrix();
}