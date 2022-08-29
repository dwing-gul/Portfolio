// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "../ProjectStruct.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <Components/WidgetComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class UNREALPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FEnemyInfo m_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* m_Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* m_Nexus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 m_Gold;

	float m_AccTime;
	float m_EndTime;

	float m_PrevHP;
	float m_PrevEP;

	bool m_bDead;
	bool m_bUnstopable;

public:
	// Sets default values for this character's properties
	AEnemy();
	~AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual bool ChangeState(EENEMY_STATE _NextState) { return true; }

	EENEMY_STATE GetState() { return m_State; }
	UBehaviorTree* GetBehaviorTree() { return m_BehaviorTree; }
	UBlackboardData* GetBlackboard() { return m_Blackboard; }
	UWidgetComponent* GetWidgetComponent() { return m_WidgetComponent; }
	AActor* GetNexus() { return m_Nexus; }
	const FEnemyInfo& GetInfo() { return m_Info; }
	bool IsUnstopable() { return m_bUnstopable; }
	bool IsDead() { return m_bDead; }

	void SetInfo(const FEnemyInfo& _Info) { m_Info = _Info; }
	void SetStopable(bool _Stopable) { m_bUnstopable = _Stopable; }
	void SetNexus(AActor* _Nexus) { m_Nexus = _Nexus; }

	void Hit(float _damage);

protected:
	void SetBlackboard(UBlackboardData* _Board) { m_Blackboard = _Board; }
	void SetBehaviorTree(UBehaviorTree* _Tree) { m_BehaviorTree = _Tree; }
	void SetState(EENEMY_STATE _State) { m_State = _State; }

private :
	void Death();

public:
	virtual void OnAttackTrigger();

public :
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
