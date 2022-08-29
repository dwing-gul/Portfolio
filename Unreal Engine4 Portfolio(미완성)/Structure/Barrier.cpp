// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "../Manager/StructureManager.h"

ABarrier::ABarrier() :
	m_State(ETURRET_STATE::IDLE),
	m_RemainTime(1.5f),
	m_AccTime(0.f)
{
}

ABarrier::~ABarrier()
{
}

void ABarrier::BeginPlay()
{
	Super::BeginPlay();

	m_MaxLevel = UStructureManager::GetInst(GetWorld())->GetMaxLv();
}

void ABarrier::Tick(float DeltaTime)
{
	if (ETURRET_STATE::DESTROY == m_State)
	{
		m_AccTime += DeltaTime;

		if (m_RemainTime < m_AccTime)
		{
			Destroy();
			return;
		}
	}

	Super::Tick(DeltaTime);

	m_Level = UStructureManager::GetInst(GetWorld())->GetCurLv(GetType());
}

void ABarrier::Destruction()
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/DownloadContents/Free_Pack/Heavy_Object_Impact_4.Heavy_Object_Impact_4'"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
	m_State = ETURRET_STATE::DESTROY;
}
