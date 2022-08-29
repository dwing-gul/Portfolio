// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NormalEnemyMeleeAAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UNormalEnemyMeleeAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_bGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_SoundEndPos;

	bool m_bActive;

public:
	UNormalEnemyMeleeAAnimInstance();
	~UNormalEnemyMeleeAAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	void PlayPhysicalBaseSound(const FString& _SocketName);

public:
	UFUNCTION()
	void AnimNotify_MeleeAAttackTrigger();

	UFUNCTION()
	void AnimNotify_MeleeAAttackEnd();

	UFUNCTION()
	void AnimNotify_MeleeAHitEnd();

	UFUNCTION()
	void AnimNotify_RightStep();

	UFUNCTION()
	void AnimNotify_LeftStep();
};
