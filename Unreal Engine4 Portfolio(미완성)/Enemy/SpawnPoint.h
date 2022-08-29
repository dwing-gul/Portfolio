// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "Enemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class UNREALPROJECT_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEnemy>	 m_SpawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 m_SpawnNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 m_MaxSpawn;

	bool m_bStart;
	int m_SpawnCount;

	TArray<AEnemy*> m_arrEnemy;
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private :
	void Spawn();

};
