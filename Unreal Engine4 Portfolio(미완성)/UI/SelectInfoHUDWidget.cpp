// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectInfoHUDWidget.h"

void USelectInfoHUDWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	m_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_HP = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPNum")));
	m_EP = Cast<UTextBlock>(GetWidgetFromName(TEXT("EPNum")));
	m_Power = Cast<UTextBlock>(GetWidgetFromName(TEXT("PowerPoint")));
	m_Speed = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedPoint")));
	m_Intellect = Cast<UTextBlock>(GetWidgetFromName(TEXT("IntellectPoint")));
	m_Health = Cast<UTextBlock>(GetWidgetFromName(TEXT("HealthPoint")));
	m_Mental = Cast<UTextBlock>(GetWidgetFromName(TEXT("MentalPoint")));
}

void USelectInfoHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
}

void USelectInfoHUDWidget::SetName(const FString& _Name)
{
	if (nullptr != m_Name)
	{
		m_Name->SetText(FText::FromString(_Name));
	}
	else
	{
		m_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
		m_Name->SetText(FText::FromString(_Name));
	}
}

void USelectInfoHUDWidget::SetHP(const FString& _HP)
{
	if (nullptr != m_HP)
	{
		m_HP->SetText(FText::FromString(_HP));
	}
	else
	{
		m_HP = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPNum")));
		m_HP->SetText(FText::FromString(_HP));
	}
}

void USelectInfoHUDWidget::SetEP(const FString& _EP)
{
	if (nullptr != m_EP)
	{
		m_EP->SetText(FText::FromString(_EP));
	}
	else
	{
		m_EP = Cast<UTextBlock>(GetWidgetFromName(TEXT("EPNum")));
		m_EP->SetText(FText::FromString(_EP));
	}
}

void USelectInfoHUDWidget::SetPower(const FString& _Power)
{
	if (nullptr != m_Power)
	{
		m_Power->SetText(FText::FromString(_Power));
	}
	else
	{
		m_Power = Cast<UTextBlock>(GetWidgetFromName(TEXT("PowerPoint")));
		m_Power->SetText(FText::FromString(_Power));
	}
}

void USelectInfoHUDWidget::SetSpeed(const FString& _Speed)
{
	if (nullptr != m_Speed)
	{
		m_Speed->SetText(FText::FromString(_Speed));
	}
	else
	{
		m_Speed = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedPoint")));
		m_Speed->SetText(FText::FromString(_Speed));
	}
}

void USelectInfoHUDWidget::SetIntellect(const FString& _Intellect)
{
	if (nullptr != m_Intellect)
	{
		m_Intellect->SetText(FText::FromString(_Intellect));
	}
	else
	{
		m_Intellect = Cast<UTextBlock>(GetWidgetFromName(TEXT("IntellectPoint")));
		m_Intellect->SetText(FText::FromString(_Intellect));
	}
}

void USelectInfoHUDWidget::SetHealth(const FString& _Health)
{
	if (nullptr != m_Health)
	{
		m_Health->SetText(FText::FromString(_Health));
	}
	else
	{
		m_Health = Cast<UTextBlock>(GetWidgetFromName(TEXT("HealthPoint")));
		m_Health->SetText(FText::FromString(_Health));
	}
}

void USelectInfoHUDWidget::SetMental(const FString& _Mental)
{
	if (nullptr != m_Mental)
	{
		m_Mental->SetText(FText::FromString(_Mental));
	}
	else
	{
		m_Mental = Cast<UTextBlock>(GetWidgetFromName(TEXT("MentalPoint")));
		m_Mental->SetText(FText::FromString(_Mental));
	}
}
