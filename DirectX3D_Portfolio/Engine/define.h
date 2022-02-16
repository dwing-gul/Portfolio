#pragma once

#define SINGLE(type)\
private :\
	type();\
	virtual ~type();\
	friend class CSingleton<type>;

#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetDeviceContext().Get()

#define GET_COMPONENT(name, NAME) C##name* name() { return (C##name*)GetComponent(COMPONENT_TYPE::NAME); }
#define GET_OTHER_COMPONENT(name) C##name* name() { return m_obj->name(); }

#define CLONE(type)\
public :\
	type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type)\
private :\
	type* Clone()\
	{\
		assert(nullptr);\
		return nullptr;\
	}\
	type(const type& _origin) = delete;

#define DELTATIME CTimeManager::GetInst()->GetDeltaTime()

#define KEY_CHECK(key, state) CKeyManager::GetInst()->GetState(key) == state
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)

#define MAX_LAYER 32
#define TEXTURE_COUNT (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0
#define UI_LAYER 29
#define DEFAULT_LAYER 0


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

enum class RESOURCE_TYPE
{
	MESHDATA,
	PREFAB,
	MATERIAL,
	GRAPHICS,
	COMPUTE,
	MESH,
	TEXTURE,
	SOUND,
	END
};

enum class CONST_BUFFER
{
	TRANSFORM,
	MATERIAL_PARAM,
	ANIM2D_INFO,
	GLOBAL,
	END
};

enum class SHADER_STAGE
{
	Vertex = 0x01,
	Hull = 0x02,
	Domain = 0x04,
	Geometry = 0x08,
	Pixel = 0x10,
	Compute = 0x20,
	NONE_PIXEL = Vertex | Hull | Domain | Geometry,
	VertexPixel = Vertex | Pixel,
	GRAPHICS = Vertex | Hull | Domain | Geometry | Pixel,
	ALL = Vertex | Hull | Domain | Geometry | Pixel | Compute
};

enum class RENDER_POV
{
	DEFERRED,
	SHADOW_DEPTH,
	LIGHT,
	FORWARD,
	PARTICLE,
	POSTEFFECT
};

enum class COMPONENT_TYPE
{
	TRANSFORM,
	BOUNDING_BOX,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	CAMERA,
	LISTENER,
	NAVIMESH,

	MESH_RENDER,
	TERRAIN,
	TILEMAP,
	PARTICLE,

	END,
	SCRIPT
};

enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
	END
};

enum class RASTERIZER_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIREFRAME,
	END
};

enum class BLEND_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ALPHABLEND_COVERAGE,
	ONE_ONE,
	END
};

enum class DEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	NO_TEST,
	NO_WRITE,
	NO_TEST_NO_WRITE,
	END
};

enum class TERRAIN_MODE
{
	HEIGHT_MAP_UP,
	HEIGHT_MAP_DOWN,
	HEIGHT_MAP_FLATTENING,
	SPLAT,
	NONE,
	END
};

enum class SHADER_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0, 
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MATRIX_0,
	MATRIX_1, 
	MATRIX_2, 
	MATRIX_3,

	TEXTURE_0,
	TEXTURE_1,
	TEXTURE_2,
	TEXTURE_3,
	TEXTURE_4,
	TEXTURE_5,
	TEXTURE_6, 
	TEXTURE_7,
	TEXTURE_ARR_0,
	TEXTURE_ARR_1,
	TEXTURE_CUBE_0,
	TEXTURE_CUBE_1,
	TEXTURE_END
};

enum class LIGHT_TYPE
{
	DIR,
	POINT,
	SPOT,
	END
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	ADD_CHILD,
	ADD_CHILD_BY_NAME,
	DISCONNECT_PARENT,
	CHANGE_SCENE,
	CHANGE_STATE,
	CHANGE_SCENE_MODE,
	END
};

enum class SCENE_MODE
{
	PLAY,
	PAUSE,
	STOP,
	END
};

enum class MRT_TYPE
{
	SWAPCHAIN,
	DEFERRED,
	LIGHT,
	SHADOW_DEPTH,
	END
};

enum class OBJECT_TYPE
{
	GENERAL,
	PLAYER,
	MONSTER,
	CAMERA,
	INTERACTION,
	LIGHT,
	NAVI_MESH,
	UI,
	END
};

enum class MOUSE_BUTTON
{
	LEFT,
	RIGHT
};

enum class PLAYER_TYPE
{
	PLAYER,
	AI,
	END
};

enum class BAR_MODE
{
	RIGHT_TO_LEFT,
	LEFT_TO_RIGHT,
	UP_TO_DOWN,
	DOWN_TO_UP,
};

enum CAMERA_TYPE
{
	MAIN,
	UI
};