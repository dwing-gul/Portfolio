// Fill out your copyright notice in the Description page of Project Settings.


#include "Twinblast.h"
#include "PlayerAnimInstance.h"
#include "../Manager/EffectManager.h"

ATwinblast::ATwinblast()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TwinblastMesh(TEXT("SkeletalMesh'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/TwinBlast.TwinBlast'"));

	if (TwinblastMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TwinblastMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprint/Player/Twinblast/ABP_Twinblast.ABP_Twinblast_C'"));

	if (AnimBlueprint.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontage(TEXT("AnimMontage'/Game/Blueprint/Player/Twinblast/AM_Twinblast_Fire.AM_Twinblast_Fire'"));

	if (FireMontage.Succeeded())
	{
		m_arrMontage.Add(FireMontage.Object);
	}
}

ATwinblast::~ATwinblast()
{
}

void ATwinblast::BeginPlay()
{
	Super::BeginPlay();
}

void ATwinblast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwinblast::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATwinblast::Attack()
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

void ATwinblast::AttackTrigger()
{
	Super::AttackTrigger();
}

void ATwinblast::HitEffect(FTransform _trans)
{
	UEffectManager::GetInst(GetWorld())->CreateEffect(EEFFECT_TYPE::HIT_BY_TWINBLAST, _trans, GetLevel());
}
