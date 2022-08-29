// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StructureBase.generated.h"

UCLASS()
class UNREALPROJECT_API AStructureBase : public APawn
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* m_Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FStructureInfo m_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESTRUCTURE_TYPE m_Type;

protected :
	float m_PrevHP;
	int32 m_MaxLevel;
	int32 m_Level;

public:	
	// Sets default values for this actor's properties
	AStructureBase();
	~AStructureBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UBoxComponent* GetBox() { return m_Box; }
	UStaticMeshComponent* GetStaticMeshComponent() { return m_StaticMesh; }
	FStructureInfo& GetInfo() { return m_Info; }
	ESTRUCTURE_TYPE GetType() { return m_Type; }
	void SetType(ESTRUCTURE_TYPE _type) { m_Type = _type; }

	void SetStaticMeshComponent(UStaticMeshComponent* _Mesh) { m_StaticMesh = _Mesh; }

	virtual void Hit(float _damage);

protected :
	virtual void Destruction() {}
};
