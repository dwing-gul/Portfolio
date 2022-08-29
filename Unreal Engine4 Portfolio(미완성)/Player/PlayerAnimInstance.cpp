// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "CharacterBase.h"
#include "../PhysicalMaterial/PhysicalMaterial_Sound.h"

UPlayerAnimInstance::UPlayerAnimInstance() :
	m_Speed(0.f),
	m_Direction(0.f),
	m_SoundEndPos(100.f),
	m_State(EPLAYER_STATE::IDLE),
	m_bActive(false)
{
}

UPlayerAnimInstance::~UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	m_bActive = true;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	ACharacterBase* character = Cast<ACharacterBase>(TryGetPawnOwner());

	if (nullptr != character && m_bActive)
	{
		UCharacterMovementComponent* movement_component = character->GetCharacterMovement();
		m_Speed = movement_component->Velocity.Size();
		m_State = character->GetState();
	}
}

void UPlayerAnimInstance::AnimNotify_AttackTrigger()
{
	ACharacterBase* character = Cast<ACharacterBase>(TryGetPawnOwner());
	if (nullptr != character)
	{
		character->AttackTrigger();
	}
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	ACharacterBase* character = Cast<ACharacterBase>(TryGetPawnOwner());

	if (nullptr != character)
	{
		character->ChangeState(EPLAYER_STATE::IDLE);
	}
}

void UPlayerAnimInstance::AnimNotify_RightStep()
{
	PlayPhysicalBaseSound(TEXT("Foot_R"));
}

void UPlayerAnimInstance::AnimNotify_LeftStep()
{
	PlayPhysicalBaseSound(TEXT("Foot_L"));
}

void UPlayerAnimInstance::PlayPhysicalBaseSound(const FString& _SocketName)
{
	ACharacterBase* character = Cast<ACharacterBase>(TryGetPawnOwner());

	if (nullptr != character && m_bActive)
	{
		FVector socket_pos = character->GetMesh()->GetSocketLocation(*_SocketName);
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