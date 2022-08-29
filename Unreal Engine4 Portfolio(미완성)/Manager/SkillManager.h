// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillManager.generated.h"

class ACharacterBase;
class ADrone;
class AGigant;
class AProjectile;

UCLASS()
class UNREALPROJECT_API USkillManager : public UObject
{
	GENERATED_BODY()
	
private :
	UTexture2D* m_arrSkillTexture[(int32)ESKILL::END];
	UTexture* m_arrSkillIcon[(int32)ESKILL_KEY::END];
	int32 m_NormalSkillUpperLevel;
	int32 m_UltimateSkillUpperLevel;

	int32 m_arrSkillLevel[(int32)ESKILL::END];
	int32 m_arrSkillLvUpPoint[(int32)ESKILL::END];
	bool m_arrSkillUsable[(int32)ESKILL::END];
	float m_arrCoolTime[(int32)ESKILL::END];
	float m_arrCoolTimer[(int32)ESKILL::END];
	float m_arrEnergy[(int32)ESKILL::END];

	bool m_bStimPack;
	bool m_bShield;

	float m_StimPackTime;
	float m_StimPackTimer;

	float m_ShieldTime;
	float m_ShieldTimer;

	AGigant* m_Gigant;

public :
	void Init();

	void SkillLevelUp(ESKILL _skill);
	void SkillLevelReset();
	int32 GetSkillLevel(ESKILL _skill) { return m_arrSkillLevel[(int32)_skill]; }
	int32 GetSkillLvUpPoint(ESKILL _skill) { return m_arrSkillLvUpPoint[(int32)_skill]; }
	int32 GetNormalSkillUpper() { return m_NormalSkillUpperLevel; }
	int32 GetUltimateSkillUpper() { return m_UltimateSkillUpperLevel; }
	
	void SetSkillIcon(ESKILL_KEY _key, ESKILL _skill) { m_arrSkillIcon[(int32)_key] = m_arrSkillTexture[(int32)_skill]; }
	UTexture* GetSkillIcon(ESKILL_KEY _key) { return m_arrSkillIcon[(int32)_key]; }
	UTexture2D* GetSkillTexture(ESKILL _Skill) { return m_arrSkillTexture[(int32)_Skill]; }

	void Skill_StimPack(int32 _level, ACharacterBase* _char);
	void Skill_DroneBomb(int32 _level, ACharacterBase* _char, TSubclassOf<ADrone> _drone);
	void Skill_EnergyShield(int32 _level, ACharacterBase* _char);
	void Skill_SatelliteBeam(int32 _level, ACharacterBase* _char, TSubclassOf<AProjectile> _Beam, float _Start);
	void Skill_CallGigant(int32 _level, ACharacterBase* _char, TSubclassOf<AGigant> _gigant);

	void SkillCoolTime(float DeltaTime);
	void StimPackTimeCheck(float DeltaTime, ACharacterBase* _char); // 스팀팩 지속시간 관리 함수
	void ShieldTimeCheck(float DeltaTime, ACharacterBase* _char); // 쉴드 지속시간 관리 함수

	bool IsSkillUsed(ESKILL _skill);
	bool IsSkillUsable(ESKILL _skill) { return m_arrSkillUsable[(int32)_skill]; }

	void ResetGigant() { m_Gigant = nullptr; }
	void Reset();
	
private :
	USkillManager();
	~USkillManager();
	
private :
	static USkillManager* m_This;
	static UWorld* m_World;

public :
	static USkillManager* GetInst(UWorld* _world);
	static void Destroy();
	
};
