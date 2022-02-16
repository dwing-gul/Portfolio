#pragma once

#define SINGLE(type)\
public :\
	static type* GetInst()\
	{\
		static type manager;\
		return &manager;\
	}\
private :\
	type();\
	~type();

#define DELTATIME TimeManager::GetInst()->GetDeltaTime()
#define CONTENT_PATH PathManager::GetInst()->GetContentPath()
#define MOUSE_POS KeyManager::GetInst()->GetMousePos()

#define KEY_TAP(key) KeyManager::GetInst()->GetState(key) == KEY_STATE::TAP
#define KEY_HOLD(key) KeyManager::GetInst()->GetState(key) == KEY_STATE::HOLD
#define KEY_AWAY(key) KeyManager::GetInst()->GetState(key) == KEY_STATE::AWAY
#define KEY_NONE(key) KeyManager::GetInst()->GetState(key) == KEY_STATE::NONE

#define CLONE(type) type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) type* Clone() { assert(nullptr); return nullptr; }

#define TILE_SIZE 64
#define JUMP_COUNT 2

enum class SCENE_TYPE
{
	TOOL,
	MAIN,
	STAGE1,
	//STAGE2,
	END
};

enum class GROUP_TYPE
{
	BACKGROUND,
	BOSS_BODY,
	TILE,
	DEFAULT,
	SUBJECT,
	NPC,
	MONSTER,
	MONSTER_PROJECTILE,
	MONSTER_HITBOX,
	ITEM,
	PLAYER,
	PLAYER_PROJECTILE,
	PLAYER_HITBOX,
	EFFECT,
	UI = 31,
	END = 32
};

enum class PEN_TYPE
{
	RED, 
	GREEN,
	BLUE,
	BLACK,
	WHITE,
	HOLLOW,
	END
};

enum class BRUSH_TYPE
{
	WHITE,
	BLACK,
	GRAY,
	HOLLOW,
	END
};

enum class UI_TYPE
{
	TILE,
	CHANGE_RENDER_MODE,
	CREATE_COLLIDER,
	SHOW_TEXTURE,
	SET_BOUNDARY,
	ABSTRACT_BOX,
	CREATE_OBJECT,
	ITEM_INFO_PANEL,
	ITEM_INFO_TEXT,
	GENERAL
};

enum class RENDER_MODE
{
	BITBLT,
	ALPHABLEND,
	TRANSPARENTBLT
};

enum class MONSTER_GROUP
{
	FOOTMAN,
	HEAVYINFANTRY,
	BOSS,
	BOSS_HEAD,
	BOSS_BODY,
	BOSS_LEFT_HAND,
	BOSS_RIGHT_HAND
};

enum class SKUL
{
	BASIC,
	BERSERKER,
	NONE
};

enum class GRADE
{
	NORMAL,
	RARE,
	UNIQUE,
	LEGENDARY
};

enum class TILE_TYPE
{
	GROUND,
	PLATFORM,
	BOUNDARY,
	NONE
};

enum class ATTACK_TYPE
{
	PHYSICAL,
	MAGICAL
};

enum class PROJECTILE_TYPE
{
	HEAD, 
	BOSS_PROJECTILE,
	BOSS_SMALL_PROJECTILE,
	PROJECTILE
};

enum class SUBJECT_TYPE
{
	DEFAULT,
	GATE,
	ITEM,
	ESSENCE,
	SKUL,
	NONE
};

enum class ITEM_TYPE
{
	ADMISSION_NOTICE_SEAT,
	RING_OF_WIND,
	THORN_PAULDRONS,
	ANCIENT_CENSER,
	BONE_HOURGLASS,
	RUBY_HEART,
	THE_BONE_OF_COURAGE,
	HOPE_CUTTER,
	PAIN_AND_DESPAIR,
	BASIC,
	BERSERKER,
	NONE // 아이템이 아님
};

enum class ENGRAVE_TYPE
{
	ENDURANCE,
	ADAPTATION,
	SCAMPER,
	SOAR,
	RAID,
	REMAINS,
	HEART,
	MUTATION,
	BRAVE,
	COLLAPSE,
	LUNACY,
	FIGHT,
	NONE // 정수이기에 각인 효과 없음
};