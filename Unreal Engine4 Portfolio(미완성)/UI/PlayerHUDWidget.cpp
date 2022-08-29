// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_EPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EPBar")));
	m_ShieldBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ShieldBar")));
	m_Portrait = Cast<UImage>(GetWidgetFromName(TEXT("Portrait")));
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
