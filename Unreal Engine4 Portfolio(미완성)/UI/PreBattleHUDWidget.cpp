// Fill out your copyright notice in the Description page of Project Settings.


#include "PreBattleHUDWidget.h"

void UPreBattleHUDWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	m_Compelete = Cast<UButton>(GetWidgetFromName(TEXT("Complete")));
	m_Back = Cast<UButton>(GetWidgetFromName(TEXT("Back")));

	m_Compelete->OnClicked.AddDynamic(this, &UPreBattleHUDWidget::CompeleteOnClicked);
	m_Back->OnClicked.AddDynamic(this, &UPreBattleHUDWidget::BackOnClicked);
}

void UPreBattleHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
}

void UPreBattleHUDWidget::CompeleteOnClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UPreBattleHUDWidget::BackOnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("CharacterSelect"));
}

