// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonsAnimInstance.h"
#include "SummonBase.h"

USummonsAnimInstance::USummonsAnimInstance()
{
}

USummonsAnimInstance::~USummonsAnimInstance()
{
}

void USummonsAnimInstance::NativeInitializeAnimation()
{
}

void USummonsAnimInstance::NativeBeginPlay()
{
}

void USummonsAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	ASummonBase* summons = Cast<ASummonBase>(TryGetPawnOwner());

	if (nullptr != summons)
	{
		m_bGround = summons->GetMovementComponent()->IsMovingOnGround();
		m_State = summons->GetState();

		if (!m_bGround)
		{
			summons->ChangeState(ESUMMONS_STATE::SPAWN);
		}
	}
}

void USummonsAnimInstance::AnimNotify_SpawnEnd()
{
	ASummonBase* summons = Cast<ASummonBase>(TryGetPawnOwner());

	if (nullptr != summons)
	{
		summons->SetStopable(false);
		summons->ChangeState(ESUMMONS_STATE::IDLE);
	}
}

void USummonsAnimInstance::AnimNotify_SummonsAttackTrigger()
{
	ASummonBase* summons = Cast<ASummonBase>(TryGetPawnOwner());

	if (nullptr != summons)
	{
		summons->OnAttackTrigger();  
	}
}

void USummonsAnimInstance::AnimNotify_SummonsAttackEnd()
{
	ASummonBase* summons = Cast<ASummonBase>(TryGetPawnOwner());

	if (nullptr != summons)
	{
		summons->SetStopable(false);
	}
}

void USummonsAnimInstance::AnimNotify_SummonsHitEnd()
{
	ASummonBase* summons = Cast<ASummonBase>(TryGetPawnOwner());

	if (nullptr != summons)
	{
		summons->SetStopable(false);
	}
}
