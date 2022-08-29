// Fill out your copyright notice in the Description page of Project Settings.


#include "Gigant.h"
#include "../Manager/SkillManager.h"

// Sets default values
AGigant::AGigant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> GigantMesh(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Prime_Helix/Meshes/Prime_Helix.Prime_Helix'"));

	if (GigantMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GigantMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/Blueprint/Summons/Gigant/ABP_Gigant.ABP_Gigant_C'"));

	if (AnimBlueprint.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);
	}

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Blueprint/Summons/BT_Summons.BT_Summons'"));

	if (BT.Succeeded())
	{
		SetBehaviorTree(BT.Object);
	}

	ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/Blueprint/Summons/BB_Summons.BB_Summons'"));

	if (BB.Succeeded())
	{
		SetBlackboard(BB.Object);
	}
}

AGigant::~AGigant()
{
	USkillManager::GetInst(GetWorld())->ResetGigant();
}

// Called when the game starts or when spawned
void AGigant::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGigant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGigant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGigant::CalGigantDamage(float _base)
{
	m_Info.AttackPower = _base * 2.f;
}

void AGigant::CalGigantHP(float _level)
{
	m_Info.MaxHP = 500.f + 500.f * _level;
	m_Info.CurHP = m_Info.MaxHP;
}