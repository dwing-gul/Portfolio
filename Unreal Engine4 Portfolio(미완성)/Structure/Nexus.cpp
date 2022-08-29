// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus.h"
#include "../UnrealProjectGameModeBase.h"

ANexus::ANexus()
{
	m_SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Model"));

	m_SkeletalMesh->SetupAttachment(GetBox());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> NexusMesh(TEXT("SkeletalMesh'/Game/DownloadContents/RTS_Scifi/Structure_v1.Structure_v1'"));

	if (NexusMesh.Succeeded())
	{
		m_SkeletalMesh->SetSkeletalMesh(NexusMesh.Object);
	}
}

ANexus::~ANexus()
{
}

void ANexus::BeginPlay()
{
	Super::BeginPlay();
}

void ANexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FStructureInfo& info = GetInfo();

	if (m_PrevHP != info.CurHP)
	{
		AUnrealProjectGameModeBase* GM = Cast<AUnrealProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if (nullptr == GM)
			return;

		float HPRatio = info.CurHP / info.MaxHP;

		GM->UpdateNexusHUD(HPRatio);
	}

	m_PrevHP = info.CurHP;
}

void ANexus::Destruction()
{

}
