// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SummonsAttack.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "SummonsAIController.h"
#include "../SummonBase.h"

UTask_SummonsAttack::UTask_SummonsAttack()
{
}

UTask_SummonsAttack::~UTask_SummonsAttack()
{
}

EBTNodeResult::Type UTask_SummonsAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASummonsAIController* Controller = Cast<ASummonsAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return EBTNodeResult::Type::Failed;

	ASummonBase* Summons = Cast<ASummonBase>(Controller->GetPawn());

	if (nullptr == Summons)
		return EBTNodeResult::Type::Succeeded;

	Summons->ChangeState(ESUMMONS_STATE::ATTACK);

	Controller->StopMovement();
	
	return EBTNodeResult::Type::Succeeded;
}

void UTask_SummonsAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_SummonsAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
