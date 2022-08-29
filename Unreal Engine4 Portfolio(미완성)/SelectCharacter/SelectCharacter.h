// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SelectCharacter.generated.h"

UCLASS()
class UNREALPROJECT_API ASelectCharacter : public ACharacter
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_bSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECLASS_TYPE m_ClassType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo m_Info;

public :
	bool IsSelect() { return m_bSelect; }
	void SetSelect(bool _bSelect) { m_bSelect = _bSelect; }

	void SetPlayerInfo(const FPlayerInfo& _info) { m_Info = _info; }
	FPlayerInfo& GetInfo() { return m_Info; }
	ECLASS_TYPE GetClassType() { return m_ClassType; }

public:
	// Sets default values for this character's properties
	ASelectCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
