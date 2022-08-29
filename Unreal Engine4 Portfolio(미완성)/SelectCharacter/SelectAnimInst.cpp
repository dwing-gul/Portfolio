// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectAnimInst.h"

void USelectAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void USelectAnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void USelectAnimInst::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	ASelectCharacter* Character = Cast<ASelectCharacter>(TryGetPawnOwner());

	if (nullptr != Character)
	{
		m_bSelect = Character->IsSelect();
	}
}

void USelectAnimInst::AnimNotify_SelectFalse()
{
	ASelectCharacter* Character = Cast<ASelectCharacter>(TryGetPawnOwner());

	if (nullptr != Character)
	{
		Character->SetSelect(false);
	}
}
