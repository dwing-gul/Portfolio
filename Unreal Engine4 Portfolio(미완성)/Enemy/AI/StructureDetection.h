// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "StructureDetection.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UStructureDetection : public UBTService
{
	GENERATED_BODY()

public:
	UStructureDetection();
	~UStructureDetection();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
