// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBase.h"
#include "../Manager/LevelStreamManager.h"

// Sets default values
AEffectBase::AEffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	SetRootComponent(m_Particle);
}

// Called when the game starts or when spawned
void AEffectBase::BeginPlay()
{
	Super::BeginPlay();
	
	UParticleSystem* ParticleAsset = Cast<UParticleSystem>(ULevelStreamManager::GetInst(GetWorld())->FindAsset(Func::EnumToName(m_Type)));

	if (nullptr != ParticleAsset)
	{
		m_Particle->SetTemplate(ParticleAsset);
		m_Particle->OnSystemFinished.AddDynamic(this, &AEffectBase::OnFinish);
	}
	else
	{
		LOG(Warning, TEXT("no"))
	}
}

// Called every frame
void AEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEffectBase::OnFinish(UParticleSystemComponent* _Particle)
{
	Destroy();
}

