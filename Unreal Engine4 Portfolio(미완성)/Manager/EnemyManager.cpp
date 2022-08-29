// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

UEnemyManager* UEnemyManager::m_This = nullptr;
UWorld* UEnemyManager::m_World = nullptr;

UEnemyManager::UEnemyManager()
{
}

UEnemyManager::~UEnemyManager()
{
}

UEnemyManager* UEnemyManager::GetInst(UWorld* _world)
{
	if (nullptr == m_This)
	{
		m_This = NewObject<UEnemyManager>();
		m_This->AddToRoot();

		m_World = _world;
	}

	if (m_World != _world)
	{
		m_World = _world;
	}

	return m_This;
}

void UEnemyManager::Destroy()
{
	if (nullptr != m_This)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}