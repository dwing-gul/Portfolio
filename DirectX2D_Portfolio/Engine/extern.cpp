#include "pch.h"
#include "extern.h"

D3D11_INPUT_ELEMENT_DESC g_LayoutDesc[5] =
{
	D3D11_INPUT_ELEMENT_DESC
	{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},

	D3D11_INPUT_ELEMENT_DESC
	{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		12,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},

	D3D11_INPUT_ELEMENT_DESC
	{
		"TANGENT",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		20,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},

	D3D11_INPUT_ELEMENT_DESC
	{
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		32,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},

	D3D11_INPUT_ELEMENT_DESC
	{
		"BINORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		44,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	}
};

const char* ComponentTypeString[(UINT)COMPONENT_TYPE::END] =
{
	"Transform",
	"BoundingBox",
	"Collider2D",
	"Collider3D",
	"Animator2D",
	"Animator3D",
	"Light2D",
	"Light3D",
	"Camera",
	"Listener",
	"MeshRender",
	"TileMap",
	"Particle"
};

const char* ResourceTypeString[(UINT)RESOURCE_TYPE::END] =
{
	"Prefab",
	"Material",
	"Graphics",
	"Compute",
	"Mesh",
	"Texture",
	"Sound"
};

const char* ProjectionTypeString[(UINT)PROJECTION_TYPE::END] =
{
	"Perspective",
	"Orthographic"
};

const char* LightTypeString[(UINT)LIGHT_TYPE::END] =
{
	"Dir",
	"Point",
	"Spot"
};

const char* ObjectTypeString[(UINT)OBJECT_TYPE::END] =
{
	"General",
	"Player",
	"Monster",
	"Camera",
	"Interaction",
	"Light",
	"UI",
};

tTransform g_transform = {};
tGlobalConst g_global = {};

const Vector3 Vector3::Right = Vector3(1.f, 0.f, 0.f);
const Vector3 Vector3::Up = Vector3(0.f, 1.f, 0.f);
const Vector3 Vector3::Front = Vector3(0.f, 0.f, 1.f);