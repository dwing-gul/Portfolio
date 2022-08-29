// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_TurretAttack.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "TurretAIController.h"
#include "../Turret.h"
#include "../../Enemy/Enemy.h"

UTask_TurretAttack::UTask_TurretAttack()
{
}

UTask_TurretAttack::~UTask_TurretAttack()
{
}

EBTNodeResult::Type UTask_TurretAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ATurretAIController* Controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return EBTNodeResult::Type::Failed;

	ATurret* Turret = Cast<ATurret>(Controller->GetPawn());

	if (nullptr == Turret)
		return EBTNodeResult::Type::Succeeded;

	Turret->ChangeState(ETURRET_STATE::ATTACK);

	return EBTNodeResult::Type::Succeeded;
}

void UTask_TurretAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_TurretAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
