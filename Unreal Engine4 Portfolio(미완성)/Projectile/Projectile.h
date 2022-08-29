// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class UNREALPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* m_ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* m_Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_Particle;

protected :
	float m_Damage;
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	~AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	UProjectileMovementComponent* GetProjectileMovement() { return m_ProjectileMovement; }
	UBoxComponent* GetBox() { return m_Box; }
	UParticleSystemComponent* GetParticle() { return m_Particle; }

protected :
	void SetProjectileMovement(UProjectileMovementComponent* _Component) { m_ProjectileMovement = _Component; }
	void SetBox(UBoxComponent* _Box) { m_Box = _Box; }
	void SetParticle(UParticleSystemComponent* _Particle) { m_Particle = _Particle; }

public:
	virtual void SetDamage(float _damage) {}
	float GetDamage() { return m_Damage; }

public :
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
