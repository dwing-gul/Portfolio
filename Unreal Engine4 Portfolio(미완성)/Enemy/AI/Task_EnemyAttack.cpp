// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_EnemyAttack.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "../Enemy.h"

UTask_EnemyAttack::UTask_EnemyAttack()
{
}

UTask_EnemyAttack::~UTask_EnemyAttack()
{
}

EBTNodeResult::Type UTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return EBTNodeResult::Type::Failed;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());

	if (nullptr == Enemy)
		return EBTNodeResult::Type::Failed;

	Enemy->ChangeState(EENEMY_STATE::ATTACK);

	Controller->StopMovement();

	return EBTNodeResult::Type::Succeeded;
}

void UTask_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_EnemyAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
