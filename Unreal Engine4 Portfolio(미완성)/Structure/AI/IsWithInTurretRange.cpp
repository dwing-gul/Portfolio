// Fill out your copyright notice in the Description page of Project Settings.


#include "IsWithInTurretRange.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "../Turret.h"
#include "../../Enemy/Enemy.h"
#include "TurretAIController.h"

UIsWithInTurretRange::UIsWithInTurretRange()
{
}

UIsWithInTurretRange::~UIsWithInTurretRange()
{
}

bool UIsWithInTurretRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bReturn = false;

	ATurret* Turret = Cast<ATurret>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == Turret)
		return false;

	if (ETURRET_STATE::DESTROY == Turret->GetState())
		return false;

	const FStructureInfo& info = Turret->GetInfo();

	ATurretAIController* Controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());

	if (nullptr == Controller)
		return false;

	AEnemy* Enemy = Cast<AEnemy>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == Enemy)
		return false;

	float Dist = FVector::Distance(Turret->GetActorLocation(), Enemy->GetActorLocation());

	if (Dist < info.AttackRange)
	{
		Turret->SetTarget(Enemy);
		bReturn = true;
	}
	else
	{
		Turret->SetTarget(nullptr);
	}

	return bReturn;
}
