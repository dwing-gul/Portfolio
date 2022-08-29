// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDetection.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "../Enemy.h"
#include "../../Player/CharacterBase.h"

UPlayerDetection::UPlayerDetection()
{
}

UPlayerDetection::~UPlayerDetection()
{
}

void UPlayerDetection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();

	if (nullptr == Controller)
		return;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());

	if (nullptr == Enemy)
		return;

	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	if (nullptr == Player)
		return;

	float Range = Enemy->GetInfo().DetectRange;
	FVector EnemyPos = Enemy->GetActorLocation();
	FVector PlayerPos = Player->GetActorLocation();

	float Dist = FVector::Distance(EnemyPos, PlayerPos);

	bool bDetect = false;
	if (Dist < Range)
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Player);
		bDetect = true;
	}
	else
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
	
//#ifdef ENABLE_DRAW_DEBUG
//	FColor color = bDetect ? FColor::Red : FColor::Green;
//	DrawDebugSphere(GetWorld(), EnemyPos, Range, 40, color, false, 0.6f);
//#endif // DEBUG
}
