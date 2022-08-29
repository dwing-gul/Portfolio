// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "Enemy.h"
#include "NormalEnemy.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ANormalEnemy : public AEnemy
{
	GENERATED_BODY()
	
public :
	ANormalEnemy();
	~ANormalEnemy();

private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Dist;

	float m_AccTime;
	float m_EndTime;

public :
	virtual bool ChangeState(EENEMY_STATE _NextState) override;

private :
	void HPVisible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
