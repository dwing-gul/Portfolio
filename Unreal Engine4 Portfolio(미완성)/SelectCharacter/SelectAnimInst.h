// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "SelectCharacter.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SelectAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USelectAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_bSelect;
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UFUNCTION()
	void AnimNotify_SelectFalse();
};
