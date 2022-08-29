// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include "../Structure/Barrier.h"
#include "../Structure/Turret.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurretSelectHUDWidget.generated.h"

UCLASS()
class UNREALPROJECT_API UTurretSelectHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
	private :
	ESTRUCTURE_TYPE m_SelectType;

	UBlueprintGeneratedClass* m_BarrierBP;
	UBlueprintGeneratedClass* m_Turret_ABP;
	UBlueprintGeneratedClass* m_Turret_BBP;
	UBlueprintGeneratedClass* m_Turret_CBP;
	UBlueprintGeneratedClass* m_Turret_DBP;

	UButton* m_Barrier;
	UButton* m_Turret_A;
	UButton* m_Turret_B;
	UButton* m_Turret_C;
	UButton* m_Turret_D;
	UButton* m_None;

	UTextBlock* m_Current;

	UButton* m_StructurePos1;
	UButton* m_StructurePos2;
	UButton* m_StructurePos3;
	UButton* m_StructurePos4;
	UButton* m_StructurePos5;
	UButton* m_StructurePos6;
	UButton* m_StructurePos7;
	UButton* m_StructurePos8;
	UButton* m_StructurePos9;
	UButton* m_StructurePos10;
	UButton* m_StructurePos11;

	AStructureBase* m_SP1;
	AStructureBase* m_SP2;
	AStructureBase* m_SP3;
	AStructureBase* m_SP4;
	AStructureBase* m_SP5;
	AStructureBase* m_SP6;
	AStructureBase* m_SP7;
	AStructureBase* m_SP8;
	AStructureBase* m_SP9;
	AStructureBase* m_SP10;
	AStructureBase* m_SP11;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public :
	UFUNCTION()
	void BarrierOnClicked();

	UFUNCTION()
	void Turret_AOnClicked();

	UFUNCTION()
	void Turret_BOnClicked();

	UFUNCTION()
	void Turret_COnClicked();

	UFUNCTION()
	void Turret_DOnClicked();

	UFUNCTION()
	void NoneOnClicked();

	UFUNCTION()
	void StructurePos1OnClicked();

	UFUNCTION()
	void StructurePos2OnClicked();

	UFUNCTION()
	void StructurePos3OnClicked();

	UFUNCTION()
	void StructurePos4OnClicked();

	UFUNCTION()
	void StructurePos5OnClicked();

	UFUNCTION()
	void StructurePos6OnClicked();

	UFUNCTION()
	void StructurePos7OnClicked();

	UFUNCTION()
	void StructurePos8OnClicked();

	UFUNCTION()
	void StructurePos9OnClicked();

	UFUNCTION()
	void StructurePos10OnClicked();

	UFUNCTION()
	void StructurePos11OnClicked();

	void SelectImage(ESTRUCTURE_TYPE _SelectType, UButton* _Widget);
	AStructureBase* SettingStructure(AStructureBase* _SP, AActor* _Setting);
};
