// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretEnemyDetection.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "../Turret.h"
#include "../../Enemy/Enemy.h"
#include "TurretAIController.h"

UTurretEnemyDetection::UTurretEnemyDetection()
{
	Interval = 1.f;
	RandomDeviation = 0.f;

	NodeName = TEXT("EnemyDetection");
}

UTurretEnemyDetection::~UTurretEnemyDetection()
{
}

void UTurretEnemyDetection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();

	if (nullptr == Controller)
		return;

	ATurret* Turret = Cast<ATurret>(Controller->GetPawn());

	if (nullptr == Turret)
		return;

	TArray<FHitResult> arrHit;
	FCollisionQueryParams param(NAME_None, false, Turret);

	FVector pos = Turret->GetActorLocation();
	float Range = Turret->GetInfo().AttackRange;

	bool bHit = Turret->GetWorld()->SweepMultiByChannel(arrHit, pos, pos, FQuat::Identity, ECC_GameTraceChannel11, FCollisionShape::MakeSphere(Range), param);

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

//#ifdef ENABLE_DRAW_DEBUG
//	FColor color = bDetect ? FColor::Red : FColor::Green;
//	DrawDebugSphere(GetWorld(), pos, Range, 40, color, false, 0.6f);
//#endif // DEBUG
}
