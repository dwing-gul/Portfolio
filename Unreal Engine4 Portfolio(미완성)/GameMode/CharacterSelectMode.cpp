// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectMode.h"
#include "../SelectCharacter/SelectController.h"

ACharacterSelectMode::ACharacterSelectMode()
{
	ConstructorHelpers::FClassFinder<UUserWidget> HUD(TEXT("WidgetBlueprint'/Game/Blueprint/UI/CharacterSelectHUD.CharacterSelectHUD_C'"));

	if (HUD.Succeeded())
	{
		m_HUDClass = HUD.Class;
	}

	PlayerControllerClass = ASelectController::StaticClass();
	DefaultPawnClass = ASelectPawn::StaticClass();
}

ACharacterSelectMode::~ACharacterSelectMode()
{
}

void ACharacterSelectMode::BeginPlay()
{
	Super::BeginPlay();

	m_HUD = Cast<UCharacterSelectHUDWidget>(CreateWidget(GetWorld(), m_HUDClass));

	if (nullptr == m_HUD)
		return;

	m_HUD->AddToViewport();

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI Mode;

	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true;
}
