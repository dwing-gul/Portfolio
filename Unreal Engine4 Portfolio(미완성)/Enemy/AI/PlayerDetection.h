// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PlayerDetection.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPlayerDetection : public UBTService
{
	GENERATED_BODY()
	
public:
	UPlayerDetection();
	~UPlayerDetection();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
