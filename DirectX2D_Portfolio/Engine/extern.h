#pragma once
#include "global.h"

extern D3D11_INPUT_ELEMENT_DESC g_LayoutDesc[5];

extern const char* ComponentTypeString[(UINT)COMPONENT_TYPE::END];
extern const char* ResourceTypeString[(UINT)RESOURCE_TYPE::END];
extern const char* ProjectionTypeString[(UINT)PROJECTION_TYPE::END];
extern const char* LightTypeString[(UINT)LIGHT_TYPE::END];
extern const char* ObjectTypeString[(UINT)OBJECT_TYPE::END];

extern tTransform g_transform;
extern tGlobalConst g_global;