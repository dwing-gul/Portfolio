// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_SoundEndPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE m_State;

	bool m_bActive;

public :
	void SetDirection(float _dir) { m_Direction = _dir; }
	float GetDirection() { return m_Direction; }

private :
	void PlayPhysicalBaseSound(const FString& _SocketName);
	
public :
	UPlayerAnimInstance();
	~UPlayerAnimInstance();

public :
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public :
	UFUNCTION()
	void AnimNotify_AttackTrigger();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_RightStep();

	UFUNCTION()
	void AnimNotify_LeftStep();
};
