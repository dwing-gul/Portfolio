// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SummonsTrace.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "SummonsAIController.h"
#include "../SummonBase.h"
#include "../../Enemy/Enemy.h"

UTask_SummonsTrace::UTask_SummonsTrace()
{
}

UTask_SummonsTrace::~UTask_SummonsTrace()
{
}

EBTNodeResult::Type UTask_SummonsTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASummonsAIController* Controller = Cast<ASummonsAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return EBTNodeResult::Type::Failed;

	ASummonBase* Summons = Cast<ASummonBase>(Controller->GetPawn());

	if (nullptr == Summons)
		return EBTNodeResult::Type::Succeeded;

	if (Summons->IsUnstopable())
	{
		return EBTNodeResult::Type::Failed;
	}

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (nullptr != Enemy)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, Enemy);
	}

	Summons->ChangeState(ESUMMONS_STATE::MOVE);

	if (Summons->GetState() == ESUMMONS_STATE::MOVE)
	{
		return EBTNodeResult::Type::Succeeded;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UTask_SummonsTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_SummonsTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
