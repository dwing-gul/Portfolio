// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAIController.h"
#include "../Turret.h"

ATurretAIController::ATurretAIController()
{
}

ATurretAIController::~ATurretAIController()
{
}

void ATurretAIController::OnPossess(APawn* _Turret)
{
	Super::OnPossess(_Turret);
	ATurret* Turret = Cast<ATurret>(_Turret);

	if (nullptr != Turret)
	{
		m_BehaviorTree = Turret->GetBehaviorTree();
		m_Blackboard = Turret->GetBlackboard();

		UseBlackboard(m_Blackboard, Blackboard);
		RunBehaviorTree(m_BehaviorTree);
	}
}

void ATurretAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
