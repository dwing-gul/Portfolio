// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "SummonBase.h"
#include "Gigant.generated.h"

UCLASS()
class UNREALPROJECT_API AGigant : public ASummonBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AGigant();
	~AGigant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	void CalGigantDamage(float _base);
	void CalGigantHP(float _level);
	void FullRecovery() { m_Info.CurHP = m_Info.MaxHP; }
};
