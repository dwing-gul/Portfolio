// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "global.h"
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEFFECT_TYPE : uint8
{
	HIT_BY_MURDOCK,
	HIT_BY_BELICA,
	HIT_BY_TWINBLAST,
	HIT_BY_NORMAL_MELEE,
	HIT_BY_TURRET,
	DESTROY_TURRET,
	DRONE_EXPLOSION,
};

#define ENUM_TO_NAME(ENUM_TYPE) static FName EnumToName(ENUM_TYPE _enum)\
								{\
									const UEnum* type = FindObject<UEnum>(ANY_PACKAGE, TEXT(#ENUM_TYPE), true);\
									FName TypeName = type->GetNameByValue((int64)_enum);\
									return TypeName;\
								}

#define ENUM_TO_STRING(ENUM_TYPE) static FString EnumToString(ENUM_TYPE _enum)\
								{\
									const UEnum* type = FindObject<UEnum>(ANY_PACKAGE, TEXT(#ENUM_TYPE), true);\
									FString TypeName = type->GetNameStringByValue((int64)_enum);\
									return TypeName;\
								}

/**
 * 
 */
class UNREALPROJECT_API Func
{
public:
	Func();
	~Func();

public :
	ENUM_TO_NAME(EEFFECT_TYPE)
	ENUM_TO_STRING(EEFFECT_TYPE)
};
