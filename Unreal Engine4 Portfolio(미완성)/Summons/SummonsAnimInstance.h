// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SummonsAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USummonsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_bGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESUMMONS_STATE m_State;

	bool m_bActive;
	
public:
	USummonsAnimInstance();
	~USummonsAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public :
	UFUNCTION()
	void AnimNotify_SpawnEnd();

	UFUNCTION()
	void AnimNotify_SummonsAttackTrigger();

	UFUNCTION()
	void AnimNotify_SummonsAttackEnd();

	UFUNCTION()
	void AnimNotify_SummonsHitEnd();
};
