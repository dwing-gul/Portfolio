// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StructureManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UStructureManager : public UObject
{
	GENERATED_BODY()
	
private :
	FStructureInfo m_arrInfo[(int32)ESTRUCTURE_TYPE::END];
	int32 m_arrStructureLv[(int32)ESTRUCTURE_TYPE::END];

	int32 m_MaxLv;
	
public:
	void Init();

	FStructureInfo& GetInfo(ESTRUCTURE_TYPE _type) { return m_arrInfo[(int)_type]; }
	int32 GetNeedGold(ESTRUCTURE_TYPE _type) { return m_arrInfo[(int)_type].Gold; }
	int32 GetCurLv(ESTRUCTURE_TYPE _type) { return m_arrStructureLv[(int)_type]; }
	int32 GetMaxLv() { return m_MaxLv; }
	float GetTurnSpeed(ESTRUCTURE_TYPE _type) { return m_arrInfo[(int)_type].TurnSpeed; }

	void AddLv(ESTRUCTURE_TYPE _type) 
	{
		++m_arrStructureLv[(int)_type]; 

		if (m_arrStructureLv[(int)_type] > m_MaxLv)
		{
			m_arrStructureLv[(int)_type] = m_MaxLv;
		}
	}

	void Reset();
	void CalStat(ESTRUCTURE_TYPE _type);
	
private:
	UStructureManager();
	~UStructureManager();

private:
	static UStructureManager* m_This;
	static UWorld* m_World;

public:
	static UStructureManager* GetInst(UWorld* _world);
	static void Destroy();
};
