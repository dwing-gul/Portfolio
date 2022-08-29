// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillHUDWidget.h"

void USkillHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_SkillImage = Cast<UImage>(GetWidgetFromName(TEXT("SkillImage")));
	m_SkillKey = Cast<UTextBlock>(GetWidgetFromName(TEXT("SkillKey")));
}

void USkillHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USkillHUDWidget::SetText(const FString& _Key)
{
	if (nullptr != m_SkillKey)
	{
		m_SkillKey->SetText(FText::FromString(_Key));
	}
	else
	{
		m_SkillKey = Cast<UTextBlock>(GetWidgetFromName(TEXT("SkillKey")));
		m_SkillKey->SetText(FText::FromString(_Key));
	}
}
