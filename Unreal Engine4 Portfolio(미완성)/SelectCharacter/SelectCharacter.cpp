// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacter.h"
#include "../ProjectGameInstance.h"

// Sets default values
ASelectCharacter::ASelectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASelectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	const FPlayerInfo* info = nullptr;
	UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!IsValid(GI))
		return;

	switch (m_ClassType)
	{
	case ECLASS_TYPE::TWINBLAST:
		info = GI->GetPlayerInfo(TEXT("Twinblast"));
		break;
	case ECLASS_TYPE::MURDOCK:
		info = GI->GetPlayerInfo(TEXT("Murdock"));
		break;
	case ECLASS_TYPE::LT_BELICA:
		info = GI->GetPlayerInfo(TEXT("LtBelica"));
		break;
	}

	m_Info = *info;
}

// Called every frame
void ASelectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASelectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

