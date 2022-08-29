// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include "CoreMinimal.h"
#include "StructureBase.h"
#include "Turret.generated.h"

class AEnemy;

UCLASS()
class UNREALPROJECT_API ATurret : public AStructureBase
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* m_Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ETURRET_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AEnemy* m_Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_RemainTime;

	float m_TurnSpeed;
	float m_AccTime;
	float m_DestroyTimer;
	USceneComponent* m_Head;

public:
	// Sets default values for this actor's properties
	ATurret();
	~ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	ETURRET_STATE GetState() { return m_State; }
	AEnemy* GetTarget() { return m_Target; }
	UBehaviorTree* GetBehaviorTree() { return m_BehaviorTree; }
	UBlackboardData* GetBlackboard() { return m_Blackboard; }
	USceneComponent* GetHead() { return m_Head; }

	void SetTarget(AEnemy* _Target) { m_Target = _Target; }

	void ChangeState(ETURRET_STATE _NextState);
	void Attack(float _DeltaTime);
	void HeadSpin(float _DeltaTime);

protected :
	virtual void Destruction() override;
};
