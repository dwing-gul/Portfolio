// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SummonsDetection.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USummonsDetection : public UBTService
{
	GENERATED_BODY()

public:
	USummonsDetection();
	~USummonsDetection();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
