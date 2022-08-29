// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeHUDWidget.h"
#include "../Player/CharacterBase.h"
#include "../Manager/SkillManager.h"
#include "../Manager/StructureManager.h"
#include "../Structure/Turret.h"
#include "../Structure/Barrier.h"
#include "Styling/SlateBrush.h"

void UUpgradeHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Power = Cast<UTextBlock>(GetWidgetFromName(TEXT("Power")));
	m_Speed = Cast<UTextBlock>(GetWidgetFromName(TEXT("Speed")));
	m_Intellect = Cast<UTextBlock>(GetWidgetFromName(TEXT("Intellect")));
	m_Health = Cast<UTextBlock>(GetWidgetFromName(TEXT("Health")));
	m_Mental = Cast<UTextBlock>(GetWidgetFromName(TEXT("Mental")));
	m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
	m_Gold = Cast<UTextBlock>(GetWidgetFromName(TEXT("Gold")));

	m_StimLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("StimLv")));
	m_DroneLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("DroneLv")));
	m_ShieldLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShieldLv")));
	m_BeamLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("BeamLv")));
	m_GigantLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("GigantLv")));

	m_BarrierLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("BarrierLv")));
	m_TurretALv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretALv")));
	m_TurretBLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretBLv")));
	m_TurretCLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretCLv")));
	m_TurretDLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretDLv")));

	m_PowerPlus = Cast<UButton>(GetWidgetFromName(TEXT("PowerPlus")));
	m_SpeedPlus = Cast<UButton>(GetWidgetFromName(TEXT("SpeedPlus")));
	m_IntellectPlus = Cast<UButton>(GetWidgetFromName(TEXT("IntellectPlus")));
	m_HealthPlus = Cast<UButton>(GetWidgetFromName(TEXT("HealthPlus")));
	m_MentalPlus = Cast<UButton>(GetWidgetFromName(TEXT("MentalPlus")));
	m_StimpackIcon = Cast<UButton>(GetWidgetFromName(TEXT("StimpackIcon")));
	m_DroneBombIcon = Cast<UButton>(GetWidgetFromName(TEXT("DroneBombIcon")));
	m_ShieldIcon = Cast<UButton>(GetWidgetFromName(TEXT("ShieldIcon")));
	m_BeamIcon = Cast<UButton>(GetWidgetFromName(TEXT("BeamIcon")));
	m_GigantIcon = Cast<UButton>(GetWidgetFromName(TEXT("GigantIcon")));
	m_Stimpack = Cast<UButton>(GetWidgetFromName(TEXT("Stimpack")));
	m_DroneBomb = Cast<UButton>(GetWidgetFromName(TEXT("DroneBomb")));
	m_Shield = Cast<UButton>(GetWidgetFromName(TEXT("Shield")));
	m_Beam = Cast<UButton>(GetWidgetFromName(TEXT("Beam")));
	m_Gigant = Cast<UButton>(GetWidgetFromName(TEXT("Gigant")));
	m_Barrier = Cast<UButton>(GetWidgetFromName(TEXT("Barrier")));
	m_Turret_A = Cast<UButton>(GetWidgetFromName(TEXT("Turret_A")));
	m_Turret_B = Cast<UButton>(GetWidgetFromName(TEXT("Turret_B")));
	m_Turret_C = Cast<UButton>(GetWidgetFromName(TEXT("Turret_C")));
	m_Turret_D = Cast<UButton>(GetWidgetFromName(TEXT("Turret_D")));

	m_QSkill = Cast<USlotHUDWidget>(GetWidgetFromName(TEXT("QSlotHUD")));
	m_ESkill = Cast<USlotHUDWidget>(GetWidgetFromName(TEXT("ESlotHUD")));
	m_RSkill = Cast<USlotHUDWidget>(GetWidgetFromName(TEXT("RSlotHUD")));

	m_Blue = LoadObject<UTexture2D>(this, TEXT("Texture2D'/Game/Image/Blue.Blue'"));
	m_White = LoadObject<UTexture2D>(this, TEXT("Texture2D'/Game/ParagonMinions/Characters/Global/T_White.T_White'"));

	if (nullptr != m_QSkill)
	{
		m_QSkill->SetKey(ESKILL_KEY::Q);
		m_QSkill->SetKeyName(TEXT("Q"));
	}

	if (nullptr != m_ESkill)
	{
		m_ESkill->SetKey(ESKILL_KEY::E);
		m_ESkill->SetKeyName(TEXT("E"));
	}

	if (nullptr != m_RSkill)
	{
		m_RSkill->SetKey(ESKILL_KEY::R);
		m_RSkill->SetKeyName(TEXT("R"));
	}

	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (nullptr != Player)
	{
		int32 point = Player->GetPoint();
		int32 gold = Player->GetGold();
		FPlayerInfo& info = Player->GetInfo();

		FString temp = FString::FromInt(point);
		temp += " p";

		if (nullptr != m_Point)
		{
			m_Point->SetText(FText::FromString(temp));
		}
		else
		{
			m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
			m_Point->SetText(FText::FromString(temp));
		}

		temp = FString::FromInt(gold);
		temp += " G";

		if (nullptr != m_Gold)
		{
			m_Gold->SetText(FText::FromString(temp));
		}
		else
		{
			m_Gold = Cast<UTextBlock>(GetWidgetFromName(TEXT("Gold")));
			m_Gold->SetText(FText::FromString(temp));
		}

		temp = FString::FromInt((int32)info.Power);

		if (nullptr != m_Power)
		{
			m_Power->SetText(FText::FromString(temp));
		}
		else
		{
			m_Power = Cast<UTextBlock>(GetWidgetFromName(TEXT("Power")));
			m_Power->SetText(FText::FromString(temp));
		}

		temp = FString::FromInt((int32)info.Speed);

		if (nullptr != m_Speed)
		{
			m_Speed->SetText(FText::FromString(temp));
		}
		else
		{
			m_Speed = Cast<UTextBlock>(GetWidgetFromName(TEXT("Speed")));
			m_Speed->SetText(FText::FromString(temp));
		}

		temp = FString::FromInt((int32)info.Intellect);

		if (nullptr != m_Intellect)
		{
			m_Intellect->SetText(FText::FromString(temp));
		}
		else
		{
			m_Intellect = Cast<UTextBlock>(GetWidgetFromName(TEXT("Intellect")));
			m_Intellect->SetText(FText::FromString(temp));
		}

		temp = FString::FromInt((int32)info.Health);

		if (nullptr != m_Health)
		{
			m_Health->SetText(FText::FromString(temp));
		}
		else
		{
			m_Health = Cast<UTextBlock>(GetWidgetFromName(TEXT("Health")));
			m_Health->SetText(FText::FromString(temp));
		}

		temp = FString::FromInt((int32)info.Mental);

		if (nullptr != m_Mental)
		{
			m_Mental->SetText(FText::FromString(temp));
		}
		else
		{
			m_Mental = Cast<UTextBlock>(GetWidgetFromName(TEXT("Mental")));
			m_Mental->SetText(FText::FromString(temp));
		}
	}

	FString NSkillLv;
	NSkillLv += TEXT("Lv ");
	NSkillLv += FString::FromInt(0);
	NSkillLv += TEXT(" / ");
	NSkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetNormalSkillUpper());

	FString USkillLv;
	USkillLv += TEXT("Lv ");
	USkillLv += FString::FromInt(0);
	USkillLv += TEXT(" / ");
	USkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetUltimateSkillUpper());

	FString StructureLv;
	StructureLv += TEXT("Lv ");
	StructureLv += FString::FromInt(0);
	StructureLv += TEXT(" / ");
	StructureLv += FString::FromInt(UStructureManager::GetInst(GetWorld())->GetMaxLv());

	if (nullptr != m_StimLv)
	{
		m_StimLv->SetText(FText::FromString(NSkillLv));
	}
	else
	{
		m_StimLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("StimLv")));
		m_StimLv->SetText(FText::FromString(NSkillLv));
	}

	if (nullptr != m_DroneLv)
	{
		m_DroneLv->SetText(FText::FromString(NSkillLv));
	}
	else
	{
		m_DroneLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("DroneLv")));
		m_DroneLv->SetText(FText::FromString(NSkillLv));
	}

	if (nullptr != m_ShieldLv)
	{
		m_ShieldLv->SetText(FText::FromString(NSkillLv));
	}
	else
	{
		m_ShieldLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShieldLv")));
		m_ShieldLv->SetText(FText::FromString(NSkillLv));
	}

	if (nullptr != m_BeamLv)
	{
		m_BeamLv->SetText(FText::FromString(USkillLv));
	}
	else
	{
		m_BeamLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("BeamLv")));
		m_BeamLv->SetText(FText::FromString(USkillLv));
	}

	if (nullptr != m_GigantLv)
	{
		m_GigantLv->SetText(FText::FromString(USkillLv));
	}
	else
	{
		m_GigantLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("GigantLv")));
		m_GigantLv->SetText(FText::FromString(USkillLv));
	}

	if (nullptr != m_BarrierLv)
	{
		m_BarrierLv->SetText(FText::FromString(StructureLv));
	}
	else
	{
		m_BarrierLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("BarrierLv")));
		m_BarrierLv->SetText(FText::FromString(StructureLv));
	}

	if (nullptr != m_TurretALv)
	{
		m_TurretALv->SetText(FText::FromString(StructureLv));
	}
	else
	{
		m_TurretALv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretALv")));
		m_TurretALv->SetText(FText::FromString(StructureLv));
	}

	if (nullptr != m_TurretBLv)
	{
		m_TurretBLv->SetText(FText::FromString(StructureLv));
	}
	else
	{
		m_TurretBLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretBLv")));
		m_TurretBLv->SetText(FText::FromString(StructureLv));
	}

	if (nullptr != m_TurretCLv)
	{
		m_TurretCLv->SetText(FText::FromString(StructureLv));
	}
	else
	{
		m_TurretCLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretCLv")));
		m_TurretCLv->SetText(FText::FromString(StructureLv));
	}

	if (nullptr != m_TurretDLv)
	{
		m_TurretDLv->SetText(FText::FromString(StructureLv));
	}
	else
	{
		m_TurretDLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurretDLv")));
		m_TurretDLv->SetText(FText::FromString(StructureLv));
	}

	m_PowerPlus->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::PowerOnClicked);
	m_SpeedPlus->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::SpeedOnClicked);
	m_IntellectPlus->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::IntellectOnClicked);
	m_HealthPlus->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::HealthOnClicked);
	m_MentalPlus->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::MentalOnClicked);
	m_StimpackIcon->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::StimpackIconOnClicked);
	m_DroneBombIcon->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::DroneBombIconOnClicked);
	m_ShieldIcon->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::ShieldIconOnClicked);
	m_BeamIcon->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::BeamIconOnClicked);
	m_GigantIcon->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::GigantIconOnClicked);
	m_Stimpack->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::StimpackOnClicked);
	m_DroneBomb->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::DroneBombOnClicked);
	m_Shield->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::ShieldOnClicked);
	m_Beam->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::BeamOnClicked);
	m_Gigant->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::GigantOnClicked);
	m_Barrier->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::BarrierOnClicked);
	m_Turret_A->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::Turret_AOnClicked);
	m_Turret_B->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::Turret_BOnClicked);
	m_Turret_C->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::Turret_COnClicked);
	m_Turret_D->OnClicked.AddDynamic(this, &UUpgradeHUDWidget::Turret_DOnClicked);
}

void UUpgradeHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (nullptr != m_QSkill)
	{
		m_QSkill->SetCurSkill(m_SelectSkill);
	}

	if (nullptr != m_ESkill)
	{
		m_ESkill->SetCurSkill(m_SelectSkill);
	}

	if (nullptr != m_RSkill)
	{
		m_RSkill->SetCurSkill(m_SelectSkill);
	}
}

void UUpgradeHUDWidget::PowerOnClicked()
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (nullptr != Player)
	{
		if (!Player->CanUsePoint(1))
			return;

		Player->SubPoint(1);

		int32 point = Player->GetPoint();
		FString temp = FString::FromInt(point);
		temp += " p";

		if (nullptr != m_Point)
		{
			m_Point->SetText(FText::FromString(temp));
		}
		else
		{
			m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
			m_Point->SetText(FText::FromString(temp));
		}

		FPlayerInfo& info = Player->GetInfo();
		++info.Power;

		temp = FString::FromInt((int)info.Power);

		if (nullptr != m_Power)
		{
			m_Power->SetText(FText::FromString(temp));
		}
		else
		{
			m_Power = Cast<UTextBlock>(GetWidgetFromName(TEXT("Power")));
			m_Power->SetText(FText::FromString(temp));
		}
	}
}

void UUpgradeHUDWidget::SpeedOnClicked()
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (nullptr != Player)
	{
		if (!Player->CanUsePoint(1))
			return;

		Player->SubPoint(1);

		int32 point = Player->GetPoint();
		FString temp = FString::FromInt(point);
		temp += " p";

		if (nullptr != m_Point)
		{
			m_Point->SetText(FText::FromString(temp));
		}
		else
		{
			m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
			m_Point->SetText(FText::FromString(temp));
		}

		FPlayerInfo& info = Player->GetInfo();
		++info.Speed;

		temp = FString::FromInt((int)info.Speed);

		if (nullptr != m_Speed)
		{
			m_Speed->SetText(FText::FromString(temp));
		}
		else
		{
			m_Speed = Cast<UTextBlock>(GetWidgetFromName(TEXT("Speed")));
			m_Speed->SetText(FText::FromString(temp));
		}
	}
}

void UUpgradeHUDWidget::IntellectOnClicked()
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (nullptr != Player)
	{
		if (!Player->CanUsePoint(1))
			return;

		Player->SubPoint(1);

		int32 point = Player->GetPoint();
		FString temp = FString::FromInt(point);
		temp += " p";

		if (nullptr != m_Point)
		{
			m_Point->SetText(FText::FromString(temp));
		}
		else
		{
			m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
			m_Point->SetText(FText::FromString(temp));
		}

		FPlayerInfo& info = Player->GetInfo();
		++info.Intellect;

		temp = FString::FromInt((int)info.Intellect);

		if (nullptr != m_Intellect)
		{
			m_Intellect->SetText(FText::FromString(temp));
		}
		else
		{
			m_Intellect = Cast<UTextBlock>(GetWidgetFromName(TEXT("Intellect")));
			m_Intellect->SetText(FText::FromString(temp));
		}
	}
}

void UUpgradeHUDWidget::HealthOnClicked()
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (nullptr != Player)
	{
		if (!Player->CanUsePoint(1))
			return;

		Player->SubPoint(1);

		int32 point = Player->GetPoint();
		FString temp = FString::FromInt(point);
		temp += " p";

		if (nullptr != m_Point)
		{
			m_Point->SetText(FText::FromString(temp));
		}
		else
		{
			m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
			m_Point->SetText(FText::FromString(temp));
		}

		FPlayerInfo& info = Player->GetInfo();
		++info.Health;

		temp = FString::FromInt((int)info.Health);

		if (nullptr != m_Health)
		{
			m_Health->SetText(FText::FromString(temp));
		}
		else
		{
			m_Health = Cast<UTextBlock>(GetWidgetFromName(TEXT("Health")));
			m_Health->SetText(FText::FromString(temp));
		}
	}
}

void UUpgradeHUDWidget::MentalOnClicked()
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (nullptr != Player)
	{
		if (!Player->CanUsePoint(1))
			return;

		Player->SubPoint(1);

		int32 point = Player->GetPoint();
		FString temp = FString::FromInt(point);
		temp += " p";

		if (nullptr != m_Point)
		{
			m_Point->SetText(FText::FromString(temp));
		}
		else
		{
			m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
			m_Point->SetText(FText::FromString(temp));
		}

		FPlayerInfo& info = Player->GetInfo();
		++info.Mental;

		temp = FString::FromInt((int)info.Mental);

		if (nullptr != m_Mental)
		{
			m_Mental->SetText(FText::FromString(temp));
		}
		else
		{
			m_Mental = Cast<UTextBlock>(GetWidgetFromName(TEXT("Mental")));
			m_Mental->SetText(FText::FromString(temp));
		}
	}
}

void UUpgradeHUDWidget::StimpackOnClicked()
{
	SkillLvUp(ESKILL::STIM_PACK);

	FString NSkillLv;
	NSkillLv += TEXT("Lv ");
	NSkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetSkillLevel(ESKILL::STIM_PACK));
	NSkillLv += TEXT(" / ");
	NSkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetNormalSkillUpper());

	if (nullptr != m_StimLv)
	{
		m_StimLv->SetText(FText::FromString(NSkillLv));
	}
	else
	{
		m_StimLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("StimLv")));
		m_StimLv->SetText(FText::FromString(NSkillLv));
	}
}

void UUpgradeHUDWidget::DroneBombOnClicked()
{
	SkillLvUp(ESKILL::DRONE_BOMB);

	FString NSkillLv;
	NSkillLv += TEXT("Lv ");
	NSkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetSkillLevel(ESKILL::DRONE_BOMB));
	NSkillLv += TEXT(" / ");
	NSkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetNormalSkillUpper());

	if (nullptr != m_DroneLv)
	{
		m_DroneLv->SetText(FText::FromString(NSkillLv));
	}
	else
	{
		m_DroneLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("DroneLv")));
		m_DroneLv->SetText(FText::FromString(NSkillLv));
	}
}

void UUpgradeHUDWidget::ShieldOnClicked()
{
	SkillLvUp(ESKILL::ENERGY_SHIELD);

	FString NSkillLv;
	NSkillLv += TEXT("Lv ");
	NSkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetSkillLevel(ESKILL::ENERGY_SHIELD));
	NSkillLv += TEXT(" / ");
	NSkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetNormalSkillUpper());

	if (nullptr != m_ShieldLv)
	{
		m_ShieldLv->SetText(FText::FromString(NSkillLv));
	}
	else
	{
		m_ShieldLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShieldLv")));
		m_ShieldLv->SetText(FText::FromString(NSkillLv));
	}
}

void UUpgradeHUDWidget::BeamOnClicked()
{
	SkillLvUp(ESKILL::SATELLITE_BEAM);

	FString USkillLv;
	USkillLv += TEXT("Lv ");
	USkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetSkillLevel(ESKILL::SATELLITE_BEAM));
	USkillLv += TEXT(" / ");
	USkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetUltimateSkillUpper());

	if (nullptr != m_BeamLv)
	{
		m_BeamLv->SetText(FText::FromString(USkillLv));
	}
	else
	{
		m_BeamLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("BeamLv")));
		m_BeamLv->SetText(FText::FromString(USkillLv));
	}
}

void UUpgradeHUDWidget::GigantOnClicked()
{
	SkillLvUp(ESKILL::CALL_GIGANT);

	FString USkillLv;
	USkillLv += TEXT("Lv ");
	USkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetSkillLevel(ESKILL::CALL_GIGANT));
	USkillLv += TEXT(" / ");
	USkillLv += FString::FromInt(USkillManager::GetInst(GetWorld())->GetUltimateSkillUpper());

	if (nullptr != m_GigantLv)
	{
		m_GigantLv->SetText(FText::FromString(USkillLv));
	}
	else
	{
		m_GigantLv = Cast<UTextBlock>(GetWidgetFromName(TEXT("GigantLv")));
		m_GigantLv->SetText(FText::FromString(USkillLv));
	}
}

void UUpgradeHUDWidget::StimpackIconOnClicked()
{
	FSlateBrush brush;

	if (m_SelectSkill != ESKILL::STIM_PACK)
	{
		m_SelectSkill = ESKILL::STIM_PACK;
		brush.SetResourceObject(m_Blue);

		FSlateBrush otherBrush;
		otherBrush.SetResourceObject(m_White);

		m_StimpackIcon->WidgetStyle.SetNormal(brush);
		m_StimpackIcon->WidgetStyle.SetHovered(brush);
		m_StimpackIcon->WidgetStyle.SetPressed(brush);

		m_DroneBombIcon->WidgetStyle.SetNormal(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetHovered(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetPressed(otherBrush);

		m_ShieldIcon->WidgetStyle.SetNormal(otherBrush);
		m_ShieldIcon->WidgetStyle.SetHovered(otherBrush);
		m_ShieldIcon->WidgetStyle.SetPressed(otherBrush);

		m_BeamIcon->WidgetStyle.SetNormal(otherBrush);
		m_BeamIcon->WidgetStyle.SetHovered(otherBrush);
		m_BeamIcon->WidgetStyle.SetPressed(otherBrush);

		m_GigantIcon->WidgetStyle.SetNormal(otherBrush);
		m_GigantIcon->WidgetStyle.SetHovered(otherBrush);
		m_GigantIcon->WidgetStyle.SetPressed(otherBrush);
	}
	else
	{
		m_SelectSkill = ESKILL::NONE;

		brush.SetResourceObject(m_White);
		m_StimpackIcon->WidgetStyle.SetNormal(brush);
		m_StimpackIcon->WidgetStyle.SetHovered(brush);
		m_StimpackIcon->WidgetStyle.SetPressed(brush);
	}
}

void UUpgradeHUDWidget::DroneBombIconOnClicked()
{
	FSlateBrush brush;

	if (m_SelectSkill != ESKILL::DRONE_BOMB)
	{
		m_SelectSkill = ESKILL::DRONE_BOMB;
		brush.SetResourceObject(m_Blue);

		FSlateBrush otherBrush;
		otherBrush.SetResourceObject(m_White);

		m_StimpackIcon->WidgetStyle.SetNormal(otherBrush);
		m_StimpackIcon->WidgetStyle.SetHovered(otherBrush);
		m_StimpackIcon->WidgetStyle.SetPressed(otherBrush);

		m_DroneBombIcon->WidgetStyle.SetNormal(brush);
		m_DroneBombIcon->WidgetStyle.SetHovered(brush);
		m_DroneBombIcon->WidgetStyle.SetPressed(brush);

		m_ShieldIcon->WidgetStyle.SetNormal(otherBrush);
		m_ShieldIcon->WidgetStyle.SetHovered(otherBrush);
		m_ShieldIcon->WidgetStyle.SetPressed(otherBrush);

		m_BeamIcon->WidgetStyle.SetNormal(otherBrush);
		m_BeamIcon->WidgetStyle.SetHovered(otherBrush);
		m_BeamIcon->WidgetStyle.SetPressed(otherBrush);

		m_GigantIcon->WidgetStyle.SetNormal(otherBrush);
		m_GigantIcon->WidgetStyle.SetHovered(otherBrush);
		m_GigantIcon->WidgetStyle.SetPressed(otherBrush);
	}
	else
	{
		m_SelectSkill = ESKILL::NONE;

		brush.SetResourceObject(m_White);
		m_DroneBombIcon->WidgetStyle.SetNormal(brush);
		m_DroneBombIcon->WidgetStyle.SetHovered(brush);
		m_DroneBombIcon->WidgetStyle.SetPressed(brush);
	}
}

void UUpgradeHUDWidget::ShieldIconOnClicked()
{
	FSlateBrush brush;

	if (m_SelectSkill != ESKILL::ENERGY_SHIELD)
	{
		m_SelectSkill = ESKILL::ENERGY_SHIELD;
		brush.SetResourceObject(m_Blue);

		FSlateBrush otherBrush;
		otherBrush.SetResourceObject(m_White);

		m_StimpackIcon->WidgetStyle.SetNormal(otherBrush);
		m_StimpackIcon->WidgetStyle.SetHovered(otherBrush);
		m_StimpackIcon->WidgetStyle.SetPressed(otherBrush);

		m_DroneBombIcon->WidgetStyle.SetNormal(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetHovered(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetPressed(otherBrush);

		m_ShieldIcon->WidgetStyle.SetNormal(brush);
		m_ShieldIcon->WidgetStyle.SetHovered(brush);
		m_ShieldIcon->WidgetStyle.SetPressed(brush);

		m_BeamIcon->WidgetStyle.SetNormal(otherBrush);
		m_BeamIcon->WidgetStyle.SetHovered(otherBrush);
		m_BeamIcon->WidgetStyle.SetPressed(otherBrush);

		m_GigantIcon->WidgetStyle.SetNormal(otherBrush);
		m_GigantIcon->WidgetStyle.SetHovered(otherBrush);
		m_GigantIcon->WidgetStyle.SetPressed(otherBrush);
	}
	else
	{
		m_SelectSkill = ESKILL::NONE;
		brush.SetResourceObject(m_White);
		m_ShieldIcon->WidgetStyle.SetNormal(brush);
		m_ShieldIcon->WidgetStyle.SetHovered(brush);
		m_ShieldIcon->WidgetStyle.SetPressed(brush);
	}
}

void UUpgradeHUDWidget::BeamIconOnClicked()
{
	FSlateBrush brush;

	if (m_SelectSkill != ESKILL::SATELLITE_BEAM)
	{
		m_SelectSkill = ESKILL::SATELLITE_BEAM;
		brush.SetResourceObject(m_Blue);

		FSlateBrush otherBrush;
		otherBrush.SetResourceObject(m_White);

		m_StimpackIcon->WidgetStyle.SetNormal(otherBrush);
		m_StimpackIcon->WidgetStyle.SetHovered(otherBrush);
		m_StimpackIcon->WidgetStyle.SetPressed(otherBrush);

		m_DroneBombIcon->WidgetStyle.SetNormal(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetHovered(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetPressed(otherBrush);

		m_ShieldIcon->WidgetStyle.SetNormal(otherBrush);
		m_ShieldIcon->WidgetStyle.SetHovered(otherBrush);
		m_ShieldIcon->WidgetStyle.SetPressed(otherBrush);

		m_BeamIcon->WidgetStyle.SetNormal(brush);
		m_BeamIcon->WidgetStyle.SetHovered(brush);
		m_BeamIcon->WidgetStyle.SetPressed(brush);

		m_GigantIcon->WidgetStyle.SetNormal(otherBrush);
		m_GigantIcon->WidgetStyle.SetHovered(otherBrush);
		m_GigantIcon->WidgetStyle.SetPressed(otherBrush);
	}
	else
	{
		m_SelectSkill = ESKILL::NONE;
		brush.SetResourceObject(m_White);
		m_BeamIcon->WidgetStyle.SetNormal(brush);
		m_BeamIcon->WidgetStyle.SetHovered(brush);
		m_BeamIcon->WidgetStyle.SetPressed(brush);
	}
}

void UUpgradeHUDWidget::GigantIconOnClicked()
{
	FSlateBrush brush;

	if (m_SelectSkill != ESKILL::CALL_GIGANT)
	{
		m_SelectSkill = ESKILL::CALL_GIGANT;
		brush.SetResourceObject(m_Blue);

		FSlateBrush otherBrush;
		otherBrush.SetResourceObject(m_White);

		m_StimpackIcon->WidgetStyle.SetNormal(otherBrush);
		m_StimpackIcon->WidgetStyle.SetHovered(otherBrush);
		m_StimpackIcon->WidgetStyle.SetPressed(otherBrush);

		m_DroneBombIcon->WidgetStyle.SetNormal(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetHovered(otherBrush);
		m_DroneBombIcon->WidgetStyle.SetPressed(otherBrush);

		m_ShieldIcon->WidgetStyle.SetNormal(otherBrush);
		m_ShieldIcon->WidgetStyle.SetHovered(otherBrush);
		m_ShieldIcon->WidgetStyle.SetPressed(otherBrush);

		m_BeamIcon->WidgetStyle.SetNormal(otherBrush);
		m_BeamIcon->WidgetStyle.SetHovered(otherBrush);
		m_BeamIcon->WidgetStyle.SetPressed(otherBrush);

		m_GigantIcon->WidgetStyle.SetNormal(brush);
		m_GigantIcon->WidgetStyle.SetHovered(brush);
		m_GigantIcon->WidgetStyle.SetPressed(brush);
	}
	else
	{
		m_SelectSkill = ESKILL::NONE;
		brush.SetResourceObject(m_White);
		m_GigantIcon->WidgetStyle.SetNormal(brush);
		m_GigantIcon->WidgetStyle.SetHovered(brush);
		m_GigantIcon->WidgetStyle.SetPressed(brush);
	}
}

void UUpgradeHUDWidget::BarrierOnClicked()
{
	StructureLvUp(ESTRUCTURE_TYPE::BARRIER, m_BarrierLv, TEXT("BarrierLv"));
}

void UUpgradeHUDWidget::Turret_AOnClicked()
{
	StructureLvUp(ESTRUCTURE_TYPE::TURRET_A, m_TurretALv, TEXT("TurretALv"));
}

void UUpgradeHUDWidget::Turret_BOnClicked()
{
	StructureLvUp(ESTRUCTURE_TYPE::TURRET_B, m_TurretBLv, TEXT("TurretBLv"));
}

void UUpgradeHUDWidget::Turret_COnClicked()
{
	StructureLvUp(ESTRUCTURE_TYPE::TURRET_C, m_TurretCLv, TEXT("TurretCLv"));
}

void UUpgradeHUDWidget::Turret_DOnClicked()
{
	StructureLvUp(ESTRUCTURE_TYPE::TURRET_D, m_TurretDLv, TEXT("TurretDLv"));
}

void UUpgradeHUDWidget::SkillLvUp(ESKILL _skill)
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	int32 LvUpPoint = USkillManager::GetInst(GetWorld())->GetSkillLvUpPoint(_skill);

	if (nullptr != Player)
	{
		if (!Player->CanUsePoint(LvUpPoint))
			return;

		Player->SubPoint(LvUpPoint);

		int32 point = Player->GetPoint();
		FString temp = FString::FromInt(point);
		temp += " p";

		if (nullptr != m_Point)
		{
			m_Point->SetText(FText::FromString(temp));
		}
		else
		{
			m_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("Point")));
			m_Point->SetText(FText::FromString(temp));
		}

		USkillManager::GetInst(GetWorld())->SkillLevelUp(_skill);
	}
}

void UUpgradeHUDWidget::StructureLvUp(ESTRUCTURE_TYPE _type, UTextBlock* _target, const FName& _name)
{
	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (nullptr != Player)
	{
		int32 Gold = UStructureManager::GetInst(GetWorld())->GetNeedGold(_type);
		int32 CurLevel = UStructureManager::GetInst(GetWorld())->GetCurLv(_type);
		int32 MaxLevel = UStructureManager::GetInst(GetWorld())->GetMaxLv();

		if (!Player->CanUseGold(Gold) || CurLevel >= MaxLevel)
		{
			return;
		}

		UStructureManager::GetInst(GetWorld())->AddLv(_type);

		CurLevel = UStructureManager::GetInst(GetWorld())->GetCurLv(_type);

		FString StructureLv;
		StructureLv += TEXT("Lv ");
		StructureLv += FString::FromInt(CurLevel);
		StructureLv += TEXT(" / ");
		StructureLv += FString::FromInt(MaxLevel);

		if (nullptr != _target)
		{
			_target->SetText(FText::FromString(StructureLv));
		}
		else
		{
			_target = Cast<UTextBlock>(GetWidgetFromName(_name));
			_target->SetText(FText::FromString(StructureLv));
		}
	}
}
