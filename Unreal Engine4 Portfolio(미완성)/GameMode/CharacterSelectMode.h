// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "../UI/CharacterSelectHUDWidget.h"
#include "../SelectCharacter/SelectPawn.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterSelectMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ACharacterSelectMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	TSubclassOf<UUserWidget> m_HUDClass;
	UCharacterSelectHUDWidget* m_HUD;

public:
	UCharacterSelectHUDWidget* GetHUD() { return m_HUD; }

public:
	ACharacterSelectMode();
	~ACharacterSelectMode();

public:
	virtual void BeginPlay() override;
};
