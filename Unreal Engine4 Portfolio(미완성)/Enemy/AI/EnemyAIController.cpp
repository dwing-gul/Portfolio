// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "../Enemy.h"

AEnemyAIController::AEnemyAIController()
{
}

AEnemyAIController::~AEnemyAIController()
{
}

void AEnemyAIController::OnPossess(APawn* _Enemy)
{
	Super::OnPossess(_Enemy);
	AEnemy* Enemy = Cast<AEnemy>(_Enemy);

	if (nullptr != Enemy)
	{
		m_BehaviorTree = Enemy->GetBehaviorTree();
		m_Blackboard = Enemy->GetBlackboard();

		UseBlackboard(m_Blackboard, Blackboard);
		RunBehaviorTree(m_BehaviorTree);
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
