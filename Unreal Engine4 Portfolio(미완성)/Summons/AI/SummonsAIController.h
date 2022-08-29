// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "SummonsAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ASummonsAIController : public AAIController
{
	GENERATED_BODY()
	
private :
	UBehaviorTree* m_BehaviorTree;
	UBlackboardData* m_Blackboard;

public :
	ASummonsAIController();
	~ASummonsAIController();

public :
	virtual void OnPossess(APawn* _Summons) override;
	virtual void OnUnPossess() override;
};
