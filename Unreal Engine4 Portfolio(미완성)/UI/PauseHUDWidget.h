// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/Button.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPauseHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	UButton* m_Resume;
	UButton* m_Restart;
	UButton* m_Exit;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public :
	UFUNCTION()
	void ResumeOnClicked();

	UFUNCTION()
	void RestartOnClicked();

	UFUNCTION()
	void ExitOnClicked();
};
