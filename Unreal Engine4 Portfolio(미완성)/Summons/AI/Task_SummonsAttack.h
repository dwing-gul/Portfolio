// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_SummonsAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UTask_SummonsAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_SummonsAttack();
	~UTask_SummonsAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
