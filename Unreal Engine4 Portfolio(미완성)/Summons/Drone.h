// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Drone.generated.h"

UCLASS()
class UNREALPROJECT_API ADrone : public ACharacter
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Drone, meta = (AllowPrivateAccess = "true"))
	float m_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Drone, meta = (AllowPrivateAccess = "true"))
	float m_ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_Dest;

	float m_Damage;

public:
	// Sets default values for this character's properties
	ADrone();
	~ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	void SetDroneDamage(float _damage);

public :
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
