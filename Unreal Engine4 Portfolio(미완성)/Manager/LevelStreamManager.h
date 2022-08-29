// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Engine/AssetManager.h>
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelStreamManager.generated.h"

USTRUCT()
struct FAssetPathInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		FName Name;

	UPROPERTY(EditAnywhere)
		FSoftObjectPath Path;
};

UCLASS(Config = Game)
class UNREALPROJECT_API ULevelStreamManager : public UObject
{
	GENERATED_BODY()
	
private:
	ULevelStreamManager();
	~ULevelStreamManager();

private:
	static ULevelStreamManager* m_This;
	static UWorld* m_World;

public:
	static ULevelStreamManager* GetInst(UWorld* _world);
	static void Destroy();

private :
	TArray<FAssetData> m_ParticleAssetData;
	TMap<FName, UObject*> m_mapAsset;

	UPROPERTY(Config)
	TArray<FAssetPathInfo> m_arrParticlePath;
	TSharedPtr<FStreamableHandle> m_ParticleHandle;

public :
	void LoadDataFromFolder(FString _path);
	void LoadAssetAsync();

	UObject* FindAsset(FName _name);

private :
	void Convert(TArray<UObject*>& _arrAssets);
	void OnCompleteParticle();
};
