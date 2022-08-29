// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseHUDWidget.h"

void UPauseHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Resume = Cast<UButton>(GetWidgetFromName(TEXT("Resume")));
	m_Restart = Cast<UButton>(GetWidgetFromName(TEXT("Restart")));
	m_Exit = Cast<UButton>(GetWidgetFromName(TEXT("Exit")));

	m_Resume->OnClicked.AddDynamic(this, &UPauseHUDWidget::ResumeOnClicked);
	m_Restart->OnClicked.AddDynamic(this, &UPauseHUDWidget::RestartOnClicked);
	m_Exit->OnClicked.AddDynamic(this, &UPauseHUDWidget::ExitOnClicked);
}

void UPauseHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPauseHUDWidget::ResumeOnClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UPauseHUDWidget::RestartOnClicked()
{
}

void UPauseHUDWidget::ExitOnClicked()
{
	
}
