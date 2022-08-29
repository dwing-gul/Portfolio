// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusHPWidget.h"

void UNexusHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_HPRatio = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPRatio")));
}

void UNexusHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UNexusHPWidget::SetText(const FString& _Ratio)
{
	if (nullptr != m_HPRatio)
	{
		m_HPRatio->SetText(FText::FromString(_Ratio));
	}
	else
	{
		m_HPRatio = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPRatio")));
		m_HPRatio->SetText(FText::FromString(_Ratio));
	}
}
