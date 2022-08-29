// Fill out your copyright notice in the Description page of Project Settings.


#include "LtBelica.h"
#include "PlayerAnimInstance.h"
#include "../Manager/EffectManager.h"

ALtBelica::ALtBelica()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> BellicaMesh(TEXT("SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica.Belica'"));

	if (BellicaMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BellicaMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprint/Player/LtBelica/ABP_LtBelica.ABP_LtBelica_C'"));

	if (AnimBlueprint.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontage(TEXT("AnimMontage'/Game/Blueprint/Player/LtBelica/AM_LtBelica_Fire.AM_LtBelica_Fire'"));

	if (FireMontage.Succeeded())
	{
		m_arrMontage.Add(FireMontage.Object);
	}
}

ALtBelica::~ALtBelica()
{
}

void ALtBelica::BeginPlay()
{
	Super::BeginPlay();
}

void ALtBelica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALtBelica::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALtBelica::Attack()
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

void ALtBelica::AttackTrigger()
{
	Super::AttackTrigger();
}

void ALtBelica::HitEffect(FTransform _trans)
{
	UEffectManager::GetInst(GetWorld())->CreateEffect(EEFFECT_TYPE::HIT_BY_BELICA, _trans, GetLevel());
}
