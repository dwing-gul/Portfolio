// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ATurretAIController : public AAIController
{
	GENERATED_BODY()

private:
	UBehaviorTree* m_BehaviorTree;
	UBlackboardData* m_Blackboard;

public:
	ATurretAIController();
	~ATurretAIController();

public:
	virtual void OnPossess(APawn* _Turret) override;
	virtual void OnUnPossess() override;
};
