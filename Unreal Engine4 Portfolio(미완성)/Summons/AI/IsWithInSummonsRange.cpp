// Fill out your copyright notice in the Description page of Project Settings.


#include "IsWithInSummonsRange.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "SummonsAIController.h"
#include "../SummonBase.h"
#include "../../Enemy/Enemy.h"

UIsWithInSummonsRange::UIsWithInSummonsRange()
{
}

UIsWithInSummonsRange::~UIsWithInSummonsRange()
{
}

bool UIsWithInSummonsRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bReturn = false;

	ASummonBase* Summons = Cast<ASummonBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Summons)
		return false;

	if (ESUMMONS_STATE::DEAD == Summons->GetState())
		return false;

	const FSummonsInfo& info = Summons->GetInfo();

	ASummonsAIController* Controller = Cast<ASummonsAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return false;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == Enemy)
		return false;

	float Dist = FVector::Distance(Summons->GetActorLocation(), Enemy->GetActorLocation());

	if (ESUMMONS_RANGE_TYPE::DETECTION == m_Type)
	{
		if (Dist < info.DetectRange)
		{
			bReturn = true;
		}
	}
	else if (ESUMMONS_RANGE_TYPE::ATTACK == m_Type)
	{
		if (Dist < info.AttackRange)
		{
			bReturn = true;
		}
	}
	
	return bReturn;
}
