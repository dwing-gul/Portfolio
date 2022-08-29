// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonsHUDWidget.h"

void USummonsHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_Portrait = Cast<UImage>(GetWidgetFromName(TEXT("Portrait")));
}

void USummonsHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
