// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureBase.h"
#include "../Manager/StructureManager.h"

// Sets default values
AStructureBase::AStructureBase() :
	m_PrevHP(0.f),
	m_MaxLevel(1),
	m_Level(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	SetRootComponent(m_Box);
}

AStructureBase::~AStructureBase()
{
}

// Called when the game starts or when spawned
void AStructureBase::BeginPlay()
{
	Super::BeginPlay();
	m_Info = UStructureManager::GetInst(GetWorld())->GetInfo(m_Type);
	m_Info.CurHP = m_Info.MaxHP;
}

// Called every frame
void AStructureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStructureBase::Hit(float _damage)
{
	m_Info.CurHP -= _damage;

	if (m_Info.CurHP <= 0.f)
	{
		Destruction();
	}
}

