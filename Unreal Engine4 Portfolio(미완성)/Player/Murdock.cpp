// Fill out your copyright notice in the Description page of Project Settings.


#include "Murdock.h"
#include "PlayerAnimInstance.h"
#include "../Manager/EffectManager.h"

AMurdock::AMurdock()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MurdockMesh(TEXT("SkeletalMesh'/Game/ParagonMurdock/Characters/Heroes/Murdock/Meshes/Murdock.Murdock'"));

	if (MurdockMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MurdockMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprint/Player/Murdock/ABP_Murdock.ABP_Murdock_C'"));

	if (AnimBlueprint.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontage(TEXT("AnimMontage'/Game/Blueprint/Player/Murdock/AM_Murdock_Fire.AM_Murdock_Fire'"));

	if (FireMontage.Succeeded())
	{
		m_arrMontage.Add(FireMontage.Object);
	}
}

AMurdock::~AMurdock()
{
}


void AMurdock::BeginPlay()
{
	Super::BeginPlay();
}

void AMurdock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMurdock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMurdock::Attack()
{
	if (m_bClicked)
	{
		GetAnimInst()->Montage_Play(m_arrMontage[0]);
	}
	else
	{
		GetAnimInst()->Montage_JumpToSection(TEXT("FireEnd"));
	}

}

void AMurdock::AttackTrigger()
{
	Super::AttackTrigger();
}

void AMurdock::HitEffect(FTransform _trans)
{
	UEffectManager::GetInst(GetWorld())->CreateEffect(EEFFECT_TYPE::HIT_BY_MURDOCK, _trans, GetLevel());
}
