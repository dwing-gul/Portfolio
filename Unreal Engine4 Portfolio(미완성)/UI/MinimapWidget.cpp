// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapWidget.h"

void UMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	m_Player = Cast<UImage>(GetWidgetFromName(TEXT("Player")));
	m_Nexus = Cast<UImage>(GetWidgetFromName(TEXT("Nexus")));
}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
