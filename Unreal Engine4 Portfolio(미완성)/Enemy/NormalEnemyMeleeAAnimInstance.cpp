// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemyMeleeAAnimInstance.h"
#include "Enemy.h"
#include "../PhysicalMaterial/PhysicalMaterial_Sound.h"

UNormalEnemyMeleeAAnimInstance::UNormalEnemyMeleeAAnimInstance()
{
}

UNormalEnemyMeleeAAnimInstance::~UNormalEnemyMeleeAAnimInstance()
{
}

void UNormalEnemyMeleeAAnimInstance::NativeInitializeAnimation()
{
}

void UNormalEnemyMeleeAAnimInstance::NativeBeginPlay()
{
}

void UNormalEnemyMeleeAAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (nullptr != enemy)
	{
		m_bGround = enemy->GetMovementComponent()->IsMovingOnGround();
		m_State = enemy->GetState();
	}
}

void UNormalEnemyMeleeAAnimInstance::PlayPhysicalBaseSound(const FString& _SocketName)
{
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (nullptr != enemy && m_bActive)
	{
		FVector socket_pos = enemy->GetMesh()->GetSocketLocation(*_SocketName);
		FVector end = socket_pos;
		end.Z -= m_SoundEndPos;

		FHitResult result;
		FCollisionQueryParams param;
		param.bReturnPhysicalMaterial = true;

		bool bHit = GetWorld()->LineTraceSingleByChannel(result, socket_pos, end, ECC_GameTraceChannel6, param);

		if (bHit)
		{
			UPhysicalMaterial_Sound* PM = Cast<UPhysicalMaterial_Sound>(result.PhysMaterial);

			if (nullptr != PM)
			{
				USoundBase* sound = PM->GetSound();

				if (nullptr != sound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound, result.ImpactPoint);
				}
			}
		}
	}
}

void UNormalEnemyMeleeAAnimInstance::AnimNotify_MeleeAAttackTrigger()
{
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (nullptr != enemy)
	{
		enemy->OnAttackTrigger();
	}
}

void UNormalEnemyMeleeAAnimInstance::AnimNotify_MeleeAAttackEnd()
{
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (nullptr != enemy)
	{
		enemy->SetStopable(false);
	}
}

void UNormalEnemyMeleeAAnimInstance::AnimNotify_MeleeAHitEnd()
{
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (nullptr != enemy)
	{
		enemy->SetStopable(false);
		
	}
}

void UNormalEnemyMeleeAAnimInstance::AnimNotify_RightStep()
{
	PlayPhysicalBaseSound(TEXT("Foot_R"));
}

void UNormalEnemyMeleeAAnimInstance::AnimNotify_LeftStep()
{
	PlayPhysicalBaseSound(TEXT("Foot_L"));
}
