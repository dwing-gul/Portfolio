// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Murdock.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AMurdock : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMurdock();
	~AMurdock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	virtual void Attack();
	virtual void AttackTrigger() override;

	virtual void HitEffect(FTransform _trans);
};
