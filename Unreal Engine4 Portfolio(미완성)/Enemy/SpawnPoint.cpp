// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"

// Sets default values
ASpawnPoint::ASpawnPoint() :
	m_bStart(false),
	m_SpawnCount(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(m_StaticMeshComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMesh.Succeeded())
	{
		m_StaticMeshComponent->SetStaticMesh(SphereMesh.Object);
	}
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bStart)
	{
		Spawn();
	}

	for (int i = 0; i < m_arrEnemy.Num(); ++i)
	{
		if (m_arrEnemy[i]->IsDead())
		{
			--m_SpawnCount;
			m_arrEnemy.RemoveAt(i);
		}
	}
}

void ASpawnPoint::Spawn()
{
	if (!m_bStart || nullptr == m_SpawnType)
		return;

	FActorSpawnParameters spawnParam = {};
	spawnParam.OverrideLevel = GetLevel();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParam.bDeferConstruction = true;

	float MaxDist = GetActorTransform().GetScale3D().X * (100.f / 2.f);
	FVector SpawnPointLocation = GetActorLocation();

	for (int i = 0; i < m_SpawnNum; ++i)
	{
		float dist = FMath::RandRange(0.f, MaxDist);
		float degree = FMath::RandRange(0.f, 2 * PI);
		FVector RandPos = FVector(FMath::Sin(degree) * dist, FMath::Cos(degree) * dist, SpawnPointLocation.Z + 100.f);
		RandPos.X += SpawnPointLocation.X;
		RandPos.Y += SpawnPointLocation.Y;

		FVector dir = RandPos - SpawnPointLocation;
		dir.Z = 0.f;
		dir.Normalize();

		if (m_SpawnCount < MaxDist)
		{
			AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(m_SpawnType, FTransform(dir.Rotation(), RandPos), spawnParam);

			Enemy->FinishSpawning(Enemy->GetTransform());

			++m_SpawnCount;
			m_arrEnemy.Add(Enemy);
		}
	}
}

