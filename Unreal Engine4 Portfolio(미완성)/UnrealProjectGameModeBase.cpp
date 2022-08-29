// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealProjectGameModeBase.h"
#include "ProjectGameInstance.h"
#include "Player/CharacterBase.h"
#include "Manager/SkillManager.h"
#include "Manager/LevelStreamManager.h"

AUnrealProjectGameModeBase::AUnrealProjectGameModeBase()
{
	UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr == GI)
		return;

	ECLASS_TYPE select_type = GI->GetSelectClassType();

	switch (select_type)
	{
	case ECLASS_TYPE::TWINBLAST:
	{
		ConstructorHelpers::FClassFinder<APawn> MainCharacterClass(TEXT("Blueprint'/Game/Blueprint/Player/Twinblast/BPC_Twinblast.BPC_Twinblast_C'"));

		if (MainCharacterClass.Succeeded())
		{
			DefaultPawnClass = MainCharacterClass.Class;
		}
	}
		break;
	case ECLASS_TYPE::MURDOCK:
	{
		ConstructorHelpers::FClassFinder<APawn> MainCharacterClass(TEXT("Blueprint'/Game/Blueprint/Player/Murdock/BPC_Murdock.BPC_Murdock_C'"));

		if (MainCharacterClass.Succeeded())
		{
			DefaultPawnClass = MainCharacterClass.Class;
		}
	}
		break;
	case ECLASS_TYPE::LT_BELICA:
	{
		ConstructorHelpers::FClassFinder<APawn> MainCharacterClass(TEXT("Blueprint'/Game/Blueprint/Player/LtBelica/BPC_LtBelica.BPC_LtBelica_C'"));

		if (MainCharacterClass.Succeeded())
		{
			DefaultPawnClass = MainCharacterClass.Class;
		}
	}
		break;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> MainHUD(TEXT("WidgetBlueprint'/Game/Blueprint/UI/MainHUD.MainHUD_C'"));

	if (MainHUD.Succeeded())
	{
		m_MainHUDClass = MainHUD.Class;
	}
}

AUnrealProjectGameModeBase::~AUnrealProjectGameModeBase()
{
}

void AUnrealProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	m_MainHUD = Cast<UMainHUDWidget>(CreateWidget(GetWorld(), m_MainHUDClass));

	if (nullptr == m_MainHUD)
		return;

	m_MainHUD->AddToViewport();

	UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr == GI)
		return;

	ECLASS_TYPE SelectType = GI->GetSelectClassType();
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	const FPlayerInfo* Info = nullptr;

	switch (SelectType)
	{
	case ECLASS_TYPE::TWINBLAST:
		break;
	case ECLASS_TYPE::MURDOCK:
		break;
	case ECLASS_TYPE::LT_BELICA:
		break;
	}

	if (nullptr != Character)
		Character;

	ULevelStreamManager::GetInst(GetWorld())->LoadAssetAsync();
}

void AUnrealProjectGameModeBase::UpdatePlayerHUD(float _CurHPRatio, float _CurEPRatio, float _CurShieldRatio)
{
	UPlayerHUDWidget* PlayerHUD = m_MainHUD->GetPlayerHUD();

	PlayerHUD->SetHP(_CurHPRatio);
	PlayerHUD->SetEP(_CurEPRatio);
	PlayerHUD->SetShield(_CurShieldRatio);
}

void AUnrealProjectGameModeBase::UpdateSummonHUD(float _CurHPRatio)
{
	USummonsHUDWidget* SummonsHUD = m_MainHUD->GetSummonsHUD();

	SummonsHUD->SetHP(_CurHPRatio);
}

void AUnrealProjectGameModeBase::UpdateNexusHUD(float _CurHPRatio)
{
	UNexusHPWidget* NexusHUD = m_MainHUD->GetNexusHUD();

	NexusHUD->SetHP(_CurHPRatio);
	FString HPPercent = FString::SanitizeFloat(_CurHPRatio * 100.f);
	HPPercent += TEXT("%");

	NexusHUD->SetText(HPPercent);
}

void AUnrealProjectGameModeBase::UpdateQSkillHUD()
{
	USkillHUDWidget* QSkillHUD = m_MainHUD->GetQSkillHUD();

	UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/UI/DepthClear_Inst.DepthClear_Inst'"));
	UMaterialInstanceDynamic* MaterialDynamic = UMaterialInstanceDynamic::Create(Material, nullptr);
	UTexture* Icon = USkillManager::GetInst(GetWorld())->GetSkillIcon(ESKILL_KEY::Q);

	if (nullptr != Icon)
	{
		QSkillHUD->SetVisibility(ESlateVisibility::Visible);
		MaterialDynamic->SetTextureParameterValue(TEXT("OutputTexture"), Icon);
		QSkillHUD->SetMaterial(MaterialDynamic);
	}
	else
	{
		QSkillHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AUnrealProjectGameModeBase::UpdateESkillHUD()
{
	USkillHUDWidget* ESkillHUD = m_MainHUD->GetESkillHUD();

	UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/UI/DepthClear_Inst.DepthClear_Inst'"));
	UMaterialInstanceDynamic* MaterialDynamic = UMaterialInstanceDynamic::Create(Material, nullptr);
	UTexture* Icon = USkillManager::GetInst(GetWorld())->GetSkillIcon(ESKILL_KEY::E);

	if (nullptr != Icon)
	{
		ESkillHUD->SetVisibility(ESlateVisibility::Visible);
		MaterialDynamic->SetTextureParameterValue(TEXT("OutputTexture"), Icon);
		ESkillHUD->SetMaterial(MaterialDynamic);
	}
	else
	{
		ESkillHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AUnrealProjectGameModeBase::UpdateRSkillHUD()
{
	USkillHUDWidget* RSkillHUD = m_MainHUD->GetRSkillHUD();

	UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/UI/DepthClear_Inst.DepthClear_Inst'"));
	UMaterialInstanceDynamic* MaterialDynamic = UMaterialInstanceDynamic::Create(Material, nullptr);
	UTexture* Icon = USkillManager::GetInst(GetWorld())->GetSkillIcon(ESKILL_KEY::R);

	if (nullptr != Icon)
	{
		RSkillHUD->SetVisibility(ESlateVisibility::Visible);
		MaterialDynamic->SetTextureParameterValue(TEXT("OutputTexture"), Icon);
		RSkillHUD->SetMaterial(MaterialDynamic);
	}
	else
	{
		RSkillHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}
