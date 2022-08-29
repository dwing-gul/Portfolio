// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "StructureBase.h"
#include "Barrier.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ABarrier : public AStructureBase
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ETURRET_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_RemainTime;

	float m_AccTime;
	
public:
	// Sets default values for this actor's properties
	ABarrier();
	~ABarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Destruction() override;
};
