// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectHUDWidget.h"
#include "../Player/CharacterBase.h"

void UCharacterSelectHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Background = Cast<UImage>(GetWidgetFromName(TEXT("Background")));
	m_SelectInfo = Cast<USelectInfoHUDWidget>(GetWidgetFromName(TEXT("SelectInfoHUD")));
	m_Compelete = Cast<UButton>(GetWidgetFromName(TEXT("Complete")));
	m_Murdock = Cast<UButton>(GetWidgetFromName(TEXT("MurdockButton")));
	m_LtBelica = Cast<UButton>(GetWidgetFromName(TEXT("LtBelicaButton")));
	m_Twinblast = Cast<UButton>(GetWidgetFromName(TEXT("TwinblastButton")));

	if (nullptr != m_Background)
	{
		UMaterialInstance* Material = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/Material/UI/DepthClear_Inst.DepthClear_Inst'"));
		UMaterialInstanceDynamic* MaterialDynamic = UMaterialInstanceDynamic::Create(Material, nullptr);
		UTexture* RenderTarget = LoadObject<UTexture>(nullptr, TEXT("TextureRenderTarget2D'/Game/Image/RenderTarget/Layout_main.Layout_Main'"));
		MaterialDynamic->SetTextureParameterValue(TEXT("OutputTexture"), RenderTarget);
		m_Background->SetBrushFromMaterial(MaterialDynamic);
	}

	if (nullptr != m_SelectInfo)
	{
		m_SelectInfo->SetName(TEXT(""));
		m_SelectInfo->SetHP(TEXT(""));
		m_SelectInfo->SetEP(TEXT(""));
		m_SelectInfo->SetPower(TEXT(""));
		m_SelectInfo->SetSpeed(TEXT(""));
		m_SelectInfo->SetIntellect(TEXT(""));
		m_SelectInfo->SetHealth(TEXT(""));
		m_SelectInfo->SetMental(TEXT(""));
	}

	m_Compelete->OnClicked.AddDynamic(this, &UCharacterSelectHUDWidget::CompeleteOnClicked);
	m_Murdock->OnClicked.AddDynamic(this, &UCharacterSelectHUDWidget::MurdockOnClicked);
	m_LtBelica->OnClicked.AddDynamic(this, &UCharacterSelectHUDWidget::LtBelicaOnClicked);
	m_Twinblast->OnClicked.AddDynamic(this, &UCharacterSelectHUDWidget::TwinblastOnClicked);
	

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASelectCharacter::StaticClass(), Actors);

	for (int32 i = 0; i < Actors.Num(); ++i)
	{
		if (Actors[i]->GetName() == TEXT("SelectMurdock"))
		{
			m_SelectMurdock = Cast<ASelectCharacter>(Actors[i]);
		}
		else if (Actors[i]->GetName() == TEXT("SelectBelica"))
		{
			m_SelectBelica = Cast<ASelectCharacter>(Actors[i]);
		}
		else if (Actors[i]->GetName() == TEXT("SelectTwinblast"))
		{
			m_SelectTwinblast = Cast<ASelectCharacter>(Actors[i]);
		}
	}
}

void UCharacterSelectHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCharacterSelectHUDWidget::CompeleteOnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainStage"));
}

void UCharacterSelectHUDWidget::MurdockOnClicked()
{
	if (nullptr != m_SelectMurdock)
	{
		m_SelectMurdock->SetActorHiddenInGame(false);
		m_SelectMurdock->SetSelect(true);
	}

	if (nullptr != m_SelectBelica)
	{
		m_SelectBelica->SetActorHiddenInGame(true);
		m_SelectBelica->SetSelect(false);
	}
	
	if (nullptr != m_SelectTwinblast)
	{
		m_SelectTwinblast->SetActorHiddenInGame(true);
		m_SelectBelica->SetSelect(false);
	}

	m_SelectInfo->SetName(TEXT("Murdock"));
	m_SelectInfo->SetHP(TEXT("300"));
	m_SelectInfo->SetEP(TEXT("120"));
	m_SelectInfo->SetPower(TEXT("20"));
	m_SelectInfo->SetSpeed(TEXT("30"));
	m_SelectInfo->SetIntellect(TEXT("15"));
	m_SelectInfo->SetHealth(TEXT("20"));
	m_SelectInfo->SetMental(TEXT("15"));
}

void UCharacterSelectHUDWidget::LtBelicaOnClicked()
{
	if (nullptr != m_SelectMurdock)
	{
		m_SelectMurdock->SetActorHiddenInGame(true);
		m_SelectMurdock->SetSelect(false);
	}

	if (nullptr != m_SelectBelica)
	{
		m_SelectBelica->SetActorHiddenInGame(false);
		m_SelectBelica->SetSelect(true);
	}

	if (nullptr != m_SelectTwinblast)
	{
		m_SelectTwinblast->SetActorHiddenInGame(true);
		m_SelectBelica->SetSelect(false);
	}

	m_SelectInfo->SetName(TEXT("LtBelica"));
	m_SelectInfo->SetHP(TEXT("250"));
	m_SelectInfo->SetEP(TEXT("150"));
	m_SelectInfo->SetPower(TEXT("15"));
	m_SelectInfo->SetSpeed(TEXT("15"));
	m_SelectInfo->SetIntellect(TEXT("30"));
	m_SelectInfo->SetHealth(TEXT("15"));
	m_SelectInfo->SetMental(TEXT("30"));
}

void UCharacterSelectHUDWidget::TwinblastOnClicked()
{
	if (nullptr != m_SelectMurdock)
	{
		m_SelectMurdock->SetActorHiddenInGame(true);
		m_SelectMurdock->SetSelect(false);
	}

	if (nullptr != m_SelectBelica)
	{
		m_SelectBelica->SetActorHiddenInGame(true);
		m_SelectBelica->SetSelect(false);
	}

	if (nullptr != m_SelectTwinblast)
	{
		m_SelectTwinblast->SetActorHiddenInGame(false);
		m_SelectBelica->SetSelect(true);
	}

	m_SelectInfo->SetName(TEXT("Twinbalst"));
	m_SelectInfo->SetHP(TEXT("400"));
	m_SelectInfo->SetEP(TEXT("80"));
	m_SelectInfo->SetPower(TEXT("35"));
	m_SelectInfo->SetSpeed(TEXT("10"));
	m_SelectInfo->SetIntellect(TEXT("10"));
	m_SelectInfo->SetHealth(TEXT("30"));
	m_SelectInfo->SetMental(TEXT("10"));
}
