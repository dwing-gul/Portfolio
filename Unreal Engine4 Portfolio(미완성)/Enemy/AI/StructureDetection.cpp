// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureDetection.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "../Enemy.h"
#include "../../Structure/StructureBase.h"

UStructureDetection::UStructureDetection()
{
}

UStructureDetection::~UStructureDetection()
{
}

void UStructureDetection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();

	if (nullptr == Controller)
		return;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());

	if (nullptr == Enemy)
		return;

	TArray<FHitResult> arrHit;
	FCollisionQueryParams param(NAME_None, false, Enemy);

	FVector Pos = Enemy->GetActorLocation();
	float Range = Enemy->GetInfo().DetectRange;

	bool bHit = Enemy->GetWorld()->SweepMultiByChannel(arrHit, Pos, Pos, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Range), param);

	bool bDetect = false;

	if (arrHit.Num())
	{
		for (int32 i = 0; i < arrHit.Num(); ++i)
		{
			AStructureBase* Structure = Cast<AStructureBase>(arrHit[i].Actor);

			if (nullptr != Structure)
			{
				FVector TargetPos = Structure->GetActorLocation();
				float Dist = FVector::Distance(Pos, TargetPos);

				if (Dist < Range)
				{
					Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Structure);
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
//	DrawDebugSphere(GetWorld(), Pos, Range, 40, color, false, 0.6f);
//#endif // DEBUG
}
