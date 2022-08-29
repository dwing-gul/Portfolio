// Fill out your copyright notice in the Description page of Project Settings.


#include "SatelliteBeam.h"
#include "../Enemy/Enemy.h"

ASatelliteBeam::ASatelliteBeam() :
	m_Time(3.f),
	m_AccTime(0.f)
{
	SetBox(CreateDefaultSubobject<UBoxComponent>(TEXT("Box")));

	m_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));

	SetParticle(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle")));

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_auroraAura.P_ky_auroraAura'"));

	if (ParticleAsset.Succeeded())
	{
		GetParticle()->SetTemplate(ParticleAsset.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("SoundCue'/Game/Blueprint/Sound/SQ_Beam.SQ_Beam'"));

	if (Sound.Succeeded())
	{
		m_Sound = Sound.Object;
	}


	SetRootComponent(GetBox());
	GetParticle()->SetupAttachment(GetBox());
	m_Sphere->SetupAttachment(GetBox());
	GetProjectileMovement()->SetUpdatedComponent(GetBox());
}

ASatelliteBeam::~ASatelliteBeam()
{
}

void ASatelliteBeam::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_Sound, GetActorLocation());
}

void ASatelliteBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_AccTime += DeltaTime;

	if (m_Time < m_AccTime)
	{
		Destroy();
	}
}

void ASatelliteBeam::SetDamage(float _damage)
{
	m_Damage = _damage * 3.f;
}

void ASatelliteBeam::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	
}
