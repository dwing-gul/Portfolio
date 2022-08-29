// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonsAIController.h"
#include "../SummonBase.h"

ASummonsAIController::ASummonsAIController()
{
}

ASummonsAIController::~ASummonsAIController()
{
}

void ASummonsAIController::OnPossess(APawn* _Summons)
{
	Super::OnPossess(_Summons);
	ASummonBase* Summons = Cast<ASummonBase>(_Summons);

	if (nullptr != Summons)
	{
		m_BehaviorTree = Summons->GetBehaviorTree();
		m_Blackboard = Summons->GetBlackboard();

		UseBlackboard(m_Blackboard, Blackboard);
		RunBehaviorTree(m_BehaviorTree);
	}
}

void ASummonsAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
