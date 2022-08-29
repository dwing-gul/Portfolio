// Fill out your copyright notice in the Description page of Project Settings.


#include "IsWithInEnemyRange.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"
#include "../Enemy.h"

UIsWithInEnemyRange::UIsWithInEnemyRange()
{
}

UIsWithInEnemyRange::~UIsWithInEnemyRange()
{
}

bool UIsWithInEnemyRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bReturn = false;

	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Enemy)
		return false;

	if (EENEMY_STATE::DEAD == Enemy->GetState())
		return false;

	const FEnemyInfo& info = Enemy->GetInfo();

	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return false;

	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == Target)
		return false;

	float Dist = FVector::Distance(Enemy->GetActorLocation(), Target->GetActorLocation());

	if (EENEMY_RANGE_TYPE::DETECTION == m_Type)
	{
		if (Dist < info.DetectRange)
		{
			bReturn = true;
		}
	}
	else if (EENEMY_RANGE_TYPE::ATTACK == m_Type)
	{
		if (Dist < info.AttackRange)
		{
			bReturn = true;
		}
	}

	return bReturn;
}
