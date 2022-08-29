// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../global.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsWithInSummonsRange.generated.h"

UENUM(BlueprintType)
enum class ESUMMONS_RANGE_TYPE : uint8
{
	DETECTION,
	ATTACK
};

UCLASS()
class UNREALPROJECT_API UIsWithInSummonsRange : public UBTDecorator
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESUMMONS_RANGE_TYPE m_Type;

public :
	UIsWithInSummonsRange();
	~UIsWithInSummonsRange();

protected :
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
