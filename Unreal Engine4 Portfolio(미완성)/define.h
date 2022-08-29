#pragma once
#include "global.h"

DECLARE_LOG_CATEGORY_EXTERN(Client, Log, All);

#define LOG(Verbose, String) UE_LOG(Client, Verbose, TEXT("%s"), *FString::Printf(TEXT("%s : { %d } : %s"), *FString(__FUNCTION__), __LINE__, String));

UENUM(BlueprintType)
enum class EPLAYER_STATE : uint8
{
	IDLE, 
	MOVE,
	ATTACK,
	HIT,
	DEAD
};

UENUM(BlueprintType)
enum class EENEMY_STATE : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	HIT,
	DEAD
};

UENUM(BlueprintType)
enum class ESUMMONS_STATE : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	HIT,
	DEAD,
	SPAWN
};

UENUM(BlueprintType)
enum class ETURRET_STATE : uint8
{
	IDLE,
	ATTACK,
	DESTROY
};

UENUM(BlueprintType)
enum class ECLASS_TYPE : uint8
{
	TWINBLAST,
	MURDOCK,
	LT_BELICA
};

UENUM(BlueprintType)
enum class EDAMAGE_TYPE : uint8
{
	BASE,
	SKILL
};

UENUM(BlueprintType)
enum class ESKILL_KEY : uint8
{
	Q,
	E,
	R,
	END
};

UENUM(BlueprintType)
enum class ESKILL : uint8
{
	NONE,
	NORMAL_SKILL,
	STIM_PACK,
	DRONE_BOMB,
	ENERGY_SHIELD,

	ULTIMATE_SKILL,
	SATELLITE_BEAM,
	CALL_GIGANT,
	END
};

UENUM(BlueprintType)
enum class ESTRUCTURE_TYPE : uint8
{
	NONE,
	BARRIER,
	TURRET_A,
	TURRET_B,
	TURRET_C,
	TURRET_D,
	NEXUS,
	END
};