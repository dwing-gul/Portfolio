// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"
#include "../Manager/SkillManager.h"
#include "../ProjectGameInstance.h"

void UMainHUDWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	m_PreBattleHUD = Cast<UPreBattleHUDWidget>(GetWidgetFromName(TEXT("PreBattleHUD")));
	m_PlayerHUD = Cast<UPlayerHUDWidget>(GetWidgetFromName(TEXT("PlayerHUD")));
	m_SummonsHUD = Cast<USummonsHUDWidget>(GetWidgetFromName(TEXT("SummonsHUD")));
	m_NexusHUD = Cast<UNexusHPWidget>(GetWidgetFromName(TEXT("NexusHP")));
	m_Q_SkillHUD = Cast<USkillHUDWidget>(GetWidgetFromName(TEXT("Q_SkillHUD")));
	m_E_SkillHUD = Cast<USkillHUDWidget>(GetWidgetFromName(TEXT("E_SkillHUD")));
	m_R_SkillHUD = Cast<USkillHUDWidget>(GetWidgetFromName(TEXT("R_SkillHUD")));
	m_Pause = Cast<UButton>(GetWidgetFromName(TEXT("Pause")));
	m_PauseHUD = Cast<UPauseHUDWidget>(GetWidgetFromName(TEXT("PauseHUD")));

	if (nullptr != m_PreBattleHUD)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

	if (nullptr != m_PlayerHUD)
	{
		UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/UI/DepthClear_Inst.DepthClear_Inst'"));
		UMaterialInstanceDynamic* MaterialDynamic = UMaterialInstanceDynamic::Create(Material, nullptr);
		UTexture* RenderTarget = nullptr;
		
		switch (GI->GetSelectClassType())
		{
		case ECLASS_TYPE::TWINBLAST:
			RenderTarget = LoadObject<UTexture>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Twinblast_Portrait.Twinblast_Portrait'"));
			break;
		case ECLASS_TYPE::MURDOCK:
			RenderTarget = LoadObject<UTexture>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Murdock_Portrait.Murdock_Portrait'"));
			break;
		case ECLASS_TYPE::LT_BELICA:
			RenderTarget = LoadObject<UTexture>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/LtBelica_Portrait.LtBelica_Portrait'"));
			break;
		}
		
		if (nullptr != RenderTarget)
		{
			MaterialDynamic->SetTextureParameterValue(TEXT("OutputTexture"), RenderTarget);
			m_PlayerHUD->SetMaterial(MaterialDynamic);
		}
	}

	if (nullptr != m_SummonsHUD)
	{
		m_SummonsHUD->SetVisibility(ESlateVisibility::Hidden);

		UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/UI/DepthClear_Inst.DepthClear_Inst'"));
		UMaterialInstanceDynamic* MaterialDynamic = UMaterialInstanceDynamic::Create(Material, nullptr);
		UTexture* RenderTarget = LoadObject<UTexture>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Gigant_Portrait.Gigant_Portrait'"));
		MaterialDynamic->SetTextureParameterValue(TEXT("OutputTexture"), RenderTarget);
		m_SummonsHUD->SetMaterial(MaterialDynamic);
	}

	if (nullptr != m_Q_SkillHUD)
	{
		m_Q_SkillHUD->SetText(TEXT("Q"));
	}

	if (nullptr != m_E_SkillHUD)
	{
		m_E_SkillHUD->SetText(TEXT("E"));
	}

	if (nullptr != m_R_SkillHUD)
	{
		m_R_SkillHUD->SetText(TEXT("R"));
	}

	if (nullptr != m_Pause)
	{
		m_Pause->OnClicked.AddDynamic(this, &UMainHUDWidget::PauseOnClicked);
	}

	if (nullptr != m_PauseHUD)
	{
		m_PauseHUD->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UMainHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
}

void UMainHUDWidget::PauseOnClicked()
{
	m_PauseHUD->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
