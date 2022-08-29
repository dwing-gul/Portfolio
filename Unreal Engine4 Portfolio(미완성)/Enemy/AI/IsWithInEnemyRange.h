// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsWithInEnemyRange.generated.h"

UENUM(BlueprintType)
enum class EENEMY_RANGE_TYPE : uint8
{
	DETECTION,
	ATTACK
};

UCLASS()
class UNREALPROJECT_API UIsWithInEnemyRange : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EENEMY_RANGE_TYPE m_Type;

public:
	UIsWithInEnemyRange();
	~UIsWithInEnemyRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
