// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ToNexus.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "../Enemy.h"
#include "../../Player/CharacterBase.h"

UTask_ToNexus::UTask_ToNexus()
{
}

UTask_ToNexus::~UTask_ToNexus()
{
}

EBTNodeResult::Type UTask_ToNexus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return EBTNodeResult::Type::Failed;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());

	if (nullptr == Enemy || EENEMY_STATE::DEAD == Enemy->GetState() || Enemy->IsUnstopable())
		return EBTNodeResult::Type::Failed;

	FVector Pos = Enemy->GetNexus()->GetActorLocation();
	Controller->GetBlackboardComponent()->SetValueAsVector(FName(TEXT("NexusPos")), Pos);

	bNotifyTick = 1;
	return EBTNodeResult::Type::InProgress;
}

void UTask_ToNexus::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());

	if (nullptr == Enemy || EENEMY_STATE::DEAD == Enemy->GetState())
		return;

	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (nullptr != Target)
	{
		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
	}
	else
	{
		FVector NexusPos = Controller->GetBlackboardComponent()->GetValueAsVector(FName(TEXT("NexusPos")));

		FVector Pos = Enemy->GetActorLocation();
		float Dist = FVector::Distance(NexusPos, Pos);
		FVector Dir = NexusPos - Pos;
		Dir.Normalize();
		Pos += Dir * Enemy->GetInfo().MoveSpeed * DeltaSeconds;

		FVector Foward = Enemy->GetActorForwardVector();
		FRotator Rot = Enemy->GetActorRotation();

		float Cos = FVector::DotProduct(Foward, Dir);
		float Radian = FMath::Acos(Cos);
		float Degree = FMath::RadiansToDegrees(Radian);

		FVector Up = Enemy->GetActorUpVector();
		FVector Cross = FVector::CrossProduct(Foward, Dir);

		if (Cross.Z > 0.f)
		{
			if (Rot.Yaw - Degree > 0.01f)
				Rot.Yaw += Degree * DeltaSeconds;
		}
		else
		{
			if (Rot.Yaw + Degree > 0.01f)
				Rot.Yaw += -Degree * DeltaSeconds;
		}

		Enemy->SetActorRotation(Rot);

		Enemy->SetActorLocation(Pos);

		Enemy->ChangeState(EENEMY_STATE::MOVE);

		if (Dist < Enemy->GetInfo().AttackRange)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
		}
	}
}

EBTNodeResult::Type UTask_ToNexus::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
