// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "SelectCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASelectController : public APlayerController
{
	GENERATED_BODY()
	
private :
	ASelectCharacter* m_PickCharacter;
	ASelectCharacter* m_PrevPickCharacter;

public :
	ASelectController();
	~ASelectController();

public :
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
};
