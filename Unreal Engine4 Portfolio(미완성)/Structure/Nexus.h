// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "StructureBase.h"
#include "Nexus.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ANexus : public AStructureBase
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_SkeletalMesh;
	
public:
	// Sets default values for this actor's properties
	ANexus();
	~ANexus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Destruction() override;

};
