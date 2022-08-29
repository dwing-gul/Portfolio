// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/TextBlock.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectInfoHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USelectInfoHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UTextBlock* m_Name;
	UTextBlock* m_HP;
	UTextBlock* m_EP;
	UTextBlock* m_Power;
	UTextBlock* m_Speed;
	UTextBlock* m_Intellect;
	UTextBlock* m_Health;
	UTextBlock* m_Mental;

public:
	void SetName(const FString& _Name);
	void SetHP(const FString& _HP);
	void SetEP(const FString& _EP);
	void SetPower(const FString& _Power);
	void SetSpeed(const FString& _Speed);
	void SetIntellect(const FString& _Intellect);
	void SetHealth(const FString& _Health);
	void SetMental(const FString& _Mental);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
