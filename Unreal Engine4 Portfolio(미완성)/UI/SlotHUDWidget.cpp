// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotHUDWidget.h"
#include "../Manager/SkillManager.h"
#include "../Player/CharacterBase.h"

void USlotHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Slot = Cast<UButton>(GetWidgetFromName(TEXT("SkillSlot")));
	m_SkillImage = Cast<UImage>(GetWidgetFromName(TEXT("SkillImage")));
	m_KeyName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Key")));

	if (nullptr != m_Slot)
	{
		m_Slot->OnClicked.AddDynamic(this, &USlotHUDWidget::SlotOnClicked);
	}
}

void USlotHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USlotHUDWidget::SlotOnClicked()
{
	UTexture2D* Icon = USkillManager::GetInst(GetWorld())->GetSkillTexture(m_CurSkill);

	m_SkillImage->SetBrushFromTexture(Icon);

	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (nullptr != Player)
	{
		Player->SkillSetting(m_Key, m_CurSkill);
	}
}

void USlotHUDWidget::SetKeyName(const FString& _Name)
{
	if (nullptr == m_KeyName)
	{
		m_KeyName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Key")));
		m_KeyName->SetText(FText::FromString(_Name));
	}
	else
	{
		m_KeyName->SetText(FText::FromString(_Name));
	}
}
