// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "../Func.h"
#include <Engine/AssetManager.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectBase.generated.h"

UCLASS()
class UNREALPROJECT_API AEffectBase : public AActor
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EEFFECT_TYPE m_Type;

	TSharedPtr<FStreamableHandle> m_Handle;

public :
	void SetEffectType(EEFFECT_TYPE _type) { m_Type = _type; }
	
public:	
	// Sets default values for this actor's properties
	AEffectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	UFUNCTION()
	void OnFinish(UParticleSystemComponent* _Particle);

};
