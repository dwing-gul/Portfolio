// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectManager.h"
#include "../Effect/EffectBase.h"

UEffectManager* UEffectManager::m_This = nullptr;
UWorld* UEffectManager::m_World = nullptr;

UEffectManager::UEffectManager()
{
}

UEffectManager::~UEffectManager()
{
}

UEffectManager* UEffectManager::GetInst(UWorld* _world)
{
	if (nullptr == m_This)
	{
		m_This = NewObject<UEffectManager>();
		m_This->AddToRoot();

		m_World = _world;
	}

	if (m_World != _world)
	{
		m_World = _world;
	}

	return m_This;
}

void UEffectManager::Destroy()
{
	if (nullptr != m_This)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}

void UEffectManager::CreateEffect(EEFFECT_TYPE _type, FTransform _trans, ULevel* _level)
{
	FActorSpawnParameters spawnParam = {};

	spawnParam.OverrideLevel = _level;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParam.bDeferConstruction = true;

	AEffectBase* effect = m_World->SpawnActor<AEffectBase>(AEffectBase::StaticClass(), _trans, spawnParam);

	effect->SetEffectType(_type);
	effect->FinishSpawning(effect->GetTransform());
}
