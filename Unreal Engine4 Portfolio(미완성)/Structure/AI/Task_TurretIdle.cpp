// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_TurretIdle.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "TurretAIController.h"
#include "../Turret.h"

UTask_TurretIdle::UTask_TurretIdle()
{
}

UTask_TurretIdle::~UTask_TurretIdle()
{
}

EBTNodeResult::Type UTask_TurretIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ATurretAIController* Controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return EBTNodeResult::Type::Failed;

	ATurret* Turret = Cast<ATurret>(Controller->GetPawn());

	if (nullptr == Turret)
		return EBTNodeResult::Type::Succeeded;

	Turret->ChangeState(ETURRET_STATE::IDLE);

	return EBTNodeResult::Type::Succeeded;
}

void UTask_TurretIdle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_TurretIdle::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
