// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsWithInTurretRange.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UIsWithInTurretRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UIsWithInTurretRange();
	~UIsWithInTurretRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
