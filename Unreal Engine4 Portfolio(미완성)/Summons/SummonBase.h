// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SummonBase.generated.h"

UCLASS()
class UNREALPROJECT_API ASummonBase : public ACharacter
{
	GENERATED_BODY()
	
protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	ESUMMONS_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSummonsInfo m_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* m_Blackboard;

	float m_AccTime;
	float m_EndTime;

	bool m_bDead;
	bool m_bUnstopable;

	float m_PrevHP;

public:
	// Sets default values for this character's properties
	ASummonBase();
	~ASummonBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	bool ChangeState(ESUMMONS_STATE _NextState);

	ESUMMONS_STATE GetState() { return m_State; }
	UBehaviorTree* GetBehaviorTree() { return m_BehaviorTree; }
	UBlackboardData* GetBlackboard() { return m_Blackboard; }
	const FSummonsInfo& GetInfo() { return m_Info; }
	bool IsUnstopable() { return m_bUnstopable; }

	void SetInfo(const FSummonsInfo& _Info) { m_Info = _Info; }
	void SetStopable(bool _Stopable) { m_bUnstopable = _Stopable; }

protected :
	void SetBlackboard(UBlackboardData* _Board) { m_Blackboard = _Board; }
	void SetBehaviorTree(UBehaviorTree* _Tree) { m_BehaviorTree = _Tree; }
	void SetState(ESUMMONS_STATE _State) { m_State = _State; }

public :
	virtual void OnAttackTrigger();
	void Hit(float _damage);
	void Death();
};
