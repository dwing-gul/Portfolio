// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamManager.h"
#include <Engine/ObjectLibrary.h>

ULevelStreamManager* ULevelStreamManager::m_This = nullptr;
UWorld* ULevelStreamManager::m_World = nullptr;

ULevelStreamManager::ULevelStreamManager()
{
}

ULevelStreamManager::~ULevelStreamManager()
{
}

ULevelStreamManager* ULevelStreamManager::GetInst(UWorld* _world)
{
	if (nullptr == m_This)
	{
		m_This = NewObject<ULevelStreamManager>();
		m_This->AddToRoot();

		m_World = _world;
	}

	if (m_World != _world)
	{
		m_World = _world;
	}

	return m_This;
}

void ULevelStreamManager::Destroy()
{
	if (nullptr != m_This)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}

void ULevelStreamManager::LoadDataFromFolder(FString _path)
{
	UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(UParticleSystem::StaticClass(), false, GIsEditor);

	if (nullptr != ObjectLibrary)
	{
		ObjectLibrary->AddToRoot();
		ObjectLibrary->LoadAssetDataFromPath(_path);
		ObjectLibrary->GetAssetDataList(m_ParticleAssetData);
	}
}

void ULevelStreamManager::LoadAssetAsync()
{
	TArray<FSoftObjectPath> arrPath;

	for (int i = 0; i < m_arrParticlePath.Num(); ++i)
	{
		arrPath.AddUnique(m_arrParticlePath[i].Path);
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	m_ParticleHandle = StreamableManager.RequestAsyncLoad(arrPath, FStreamableDelegate::CreateUObject(this, &ULevelStreamManager::OnCompleteParticle));
}

UObject* ULevelStreamManager::FindAsset(FName _name)
{
	UObject* Asset = m_mapAsset.FindRef(_name);
	return Asset;
}

void ULevelStreamManager::Convert(TArray<UObject*>& _arrAssets)
{
	for (int i = 0; i < m_arrParticlePath.Num(); ++i)
	{
		m_mapAsset.Add(m_arrParticlePath[i].Name, _arrAssets[i]);
	}
}

void ULevelStreamManager::OnCompleteParticle()
{
	TArray<UObject*> arrAssets;
	m_ParticleHandle->GetLoadedAssets(arrAssets);

	Convert(arrAssets);
}
