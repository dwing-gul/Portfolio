// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDetection.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "SummonsAIController.h"
#include "../../Enemy/Enemy.h"
#include "../SummonBase.h"

UEnemyDetection::UEnemyDetection()
{
	Interval = 1.f;
	RandomDeviation = 0.f;

	NodeName = TEXT("EnemyDetection");
}

UEnemyDetection::~UEnemyDetection()
{
}

void UEnemyDetection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();

	if (nullptr == Controller)
		return;

	ASummonBase* Summons = Cast<ASummonBase>(Controller->GetPawn());

	if (nullptr == Summons)
		return;

	if (!Summons->GetMovementComponent()->IsMovingOnGround())
	{
		return;
	}

	TArray<FHitResult> arrHit;
	FCollisionQueryParams param(NAME_None, false, Summons);

	FVector pos = Summons->GetActorLocation();
	float Range = Summons->GetInfo().DetectRange;

	bool bHit = Summons->GetWorld()->SweepMultiByChannel(arrHit, pos, pos, FQuat::Identity, ECC_GameTraceChannel7, FCollisionShape::MakeSphere(Range), param);
	
	bool bDetect = false;

	if (arrHit.Num())
	{
		for (int32 i = 0; i < arrHit.Num(); ++i)
		{
			AEnemy* Enemy = Cast<AEnemy>(arrHit[i].Actor);

			if (nullptr != Enemy)
			{
				FVector TargetPos = Enemy->GetActorLocation();
				float Dist = FVector::Distance(pos, TargetPos);

				if (Dist < Range)
				{
					Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Enemy);
					bDetect = true;
					break;
				}
				else
				{
					Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
				}
			}
		}
	}

#ifdef ENABLE_DRAW_DEBUG
	FColor color = bDetect ? FColor::Red : FColor::Green;
	DrawDebugSphere(GetWorld(), pos, Range, 40, color, false, 0.6f);
#endif // DEBUG
}
