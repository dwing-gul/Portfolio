// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_EnemyTrace.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "../Enemy.h"

UTask_EnemyTrace::UTask_EnemyTrace()
{
}

UTask_EnemyTrace::~UTask_EnemyTrace()
{
}

EBTNodeResult::Type UTask_EnemyTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return EBTNodeResult::Type::Failed;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());

	if (nullptr == Enemy)
		return EBTNodeResult::Type::Succeeded;

	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	Enemy->ChangeState(EENEMY_STATE::MOVE);

	if (Enemy->GetState() != EENEMY_STATE::MOVE)
	{
		return EBTNodeResult::Type::Succeeded;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, Target);

	return EBTNodeResult::Type::Succeeded;
}

void UTask_EnemyTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_EnemyTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
