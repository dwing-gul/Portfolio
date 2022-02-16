#pragma once

#define SINGLE(type)\
private :\
	type();\
	virtual ~type();\
	friend class Singleton<type>;

#define DEVICE Device::GetInst()->GetDevice()
#define CONTEXT Device::GetInst()->GetDeviceContext()

#define GET_COMPONENT(name, NAME) name* Get##name() { return (name*)GetComponent(COMPONENT_TYPE::NAME); }
#define GET_OTHER_COMPONENT(name) name* Get##name() { return m_obj->Get##name(); }

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

#define DELTATIME TimeManager::GetInst()->GetDeltaTime()
#define KEY_CHECK(key, state) KeyManager::GetInst()->GetState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)

#define MAX_LAYER 32

enum class RESOURCE_TYPE
{
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

enum class COMPONENT_TYPE
{
	TRANSFORM,
	BOUNDINGBOX,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	CAMERA,
	LISTENER,

	MESH_RENDER,
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

	VECTOR2_0,
	VECTOR2_1,
	VECTOR2_2,
	VECTOR2_3,

	VECTOR4_0,
	VECTOR4_1,
	VECTOR4_2,
	VECTOR4_3,

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
	TEXTURE_END,
};

enum class RENDER_POV
{
	TILEMAP,
	FORWARD,
	PARTICLE,
	POST_EFFECT
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
	CREATE_OBJECT_BY_SCENE,
	DELETE_OBJECT,
	ADD_CHILD,
	DISCONNECT_PARENT,
	CHANGE_SCENE,
	CHANGE_STATE, // AI 상태 변환
	CHANGE_SCENE_MODE
};

enum class SCENE_MODE
{
	PLAY,
	PAUSE,
	STOP
};

enum class MOUSE_BUTTON
{
	LEFT,
	RIGHT
};

enum class COLLIDER_TYPE
{
	WALL,
	INTERACTION,
	INTERACTION_DOOR,
	INTERACTION_BATTLE_ZONE,
	OBJECT
};

enum class OBJECT_TYPE
{
	GENERAL,
	PLAYER,
	MONSTER,
	CAMERA,
	INTERACTION,
	LIGHT,
	UI,
	END
};
