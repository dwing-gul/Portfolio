// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicalMaterial_Sound.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPhysicalMaterial_Sound : public UPhysicalMaterial
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase* m_Sound;

public :
	void SetSound(USoundBase* _Sound) { m_Sound = _Sound; }

	USoundBase* GetSound() { return m_Sound; }
};
