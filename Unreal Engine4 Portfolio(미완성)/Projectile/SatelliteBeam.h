// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "Projectile.h"
#include "SatelliteBeam.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASatelliteBeam : public AProjectile
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Distance, meta = (AllowPrivateAccess = "true"))
	float m_Time;

	float m_AccTime;
	USoundBase* m_Sound;
	
public:
	// Sets default values for this actor's properties
	ASatelliteBeam();
	~ASatelliteBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetDamage(float _damage) override;

public:
	virtual void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
