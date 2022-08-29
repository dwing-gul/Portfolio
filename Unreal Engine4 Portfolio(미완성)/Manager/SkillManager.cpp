// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManager.h"
#include "../Player/CharacterBase.h"
#include "../Summons/Drone.h"
#include "../Summons/Gigant.h"
#include "../Projectile/SatelliteBeam.h"

USkillManager* USkillManager::m_This = nullptr;
UWorld* USkillManager::m_World = nullptr;

USkillManager::USkillManager()
{
}

USkillManager::~USkillManager()
{
}

USkillManager* USkillManager::GetInst(UWorld* _world)
{
	if (nullptr == m_This)
	{
		m_This = NewObject<USkillManager>();
		m_This->AddToRoot();

		m_World = _world;
	}

	if (m_World != _world)
	{
		m_World = _world;
	}

	return m_This;
}

void USkillManager::Destroy()
{
	if (nullptr != m_This)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}

void USkillManager::Init()
{
	m_NormalSkillUpperLevel = 5;
	m_UltimateSkillUpperLevel = 3;
	m_bStimPack = false;
	m_bShield = false;
	m_StimPackTime = 3.f;
	m_StimPackTimer = 0.f;
	m_ShieldTime = 10.f;
	m_ShieldTimer = 0.f;
	m_Gigant = nullptr;

	for (int32 i = 0; i < (int32)ESKILL::END; ++i)
	{
		m_arrSkillLevel[i] = 0;
		m_arrSkillUsable[i] = true;
		m_arrCoolTimer[i] = 0.f;

		switch ((ESKILL)i)
		{
		case ESKILL::NONE :
			m_arrSkillTexture[i] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Engine/EngineResources/Gray.Gray'"));
			break;
		case ESKILL::STIM_PACK:
			m_arrSkillTexture[i] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/DownloadContents/Skill_UI/UI_Skill_Icon_Fly.UI_Skill_Icon_Fly'"));
			m_arrCoolTime[i] = 25.f;
			m_arrEnergy[i] = 10.f;
			m_arrSkillLvUpPoint[i] = 2;
			break;
		case ESKILL::DRONE_BOMB:
			m_arrSkillTexture[i] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/DownloadContents/Skill_UI/UI_Skill_Icon_MeteorShower.UI_Skill_Icon_MeteorShower'"));
			m_arrCoolTime[i] = 15.f;
			m_arrEnergy[i] = 8.f;
			m_arrSkillLvUpPoint[i] = 1;
			break;
		case ESKILL::ENERGY_SHIELD:
			m_arrSkillTexture[i] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/DownloadContents/Skill_UI/UI_Skill_Icon_Reflect.UI_Skill_Icon_Reflect'"));
			m_arrCoolTime[i] = 20.f;
			m_arrEnergy[i] = 30.f;
			m_arrSkillLvUpPoint[i] = 2;
			break;
		case ESKILL::SATELLITE_BEAM:
			m_arrSkillTexture[i] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/DownloadContents/Skill_UI/UI_Skill_Icon_Beam.UI_Skill_Icon_Beam'"));
			m_arrCoolTime[i] = 80.f;
			m_arrEnergy[i] = 100.f;
			m_arrSkillLvUpPoint[i] = 4;
			break;
		case ESKILL::CALL_GIGANT:
			m_arrSkillTexture[i] = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/DownloadContents/Skill_UI/UI_Skill_Icon_Buff.UI_Skill_Icon_Buff'"));
			m_arrCoolTime[i] = 90.f;
			m_arrEnergy[i] = 150.f;
			m_arrSkillLvUpPoint[i] = 5;
			break;
		}
	}
}

void USkillManager::SkillLevelUp(ESKILL _skill)
{
	++m_arrSkillLevel[(int32)_skill];

	switch (_skill)
	{
	case ESKILL::STIM_PACK:
	case ESKILL::DRONE_BOMB:
	case ESKILL::ENERGY_SHIELD:
	{
		if (m_NormalSkillUpperLevel < m_arrSkillLevel[(int32)_skill])
		{
			m_arrSkillLevel[(int32)_skill] = m_NormalSkillUpperLevel;
		}
	}
	break;
	case ESKILL::SATELLITE_BEAM:
	case ESKILL::CALL_GIGANT:
	{
		if (m_UltimateSkillUpperLevel < m_arrSkillLevel[(int32)_skill])
		{
			m_arrSkillLevel[(int32)_skill] = m_UltimateSkillUpperLevel;
		}
	}
	break;
	}
}

void USkillManager::SkillLevelReset()
{
	for (int32 i = 0; i < (int32)ESKILL::END; ++i)
	{
		m_arrSkillLevel[i] = 0;
	}
}

void USkillManager::Skill_StimPack(int32 _level, ACharacterBase* _char)
{
	if (0 == _level)
		return;

	if (nullptr != _char && !m_bStimPack && m_arrSkillUsable[(int32)ESKILL::STIM_PACK])
	{
		m_bStimPack = true;
		m_arrSkillUsable[(int32)ESKILL::STIM_PACK] = false;
		float AttackSpeed = _char->GetAttackSpeed();
		float MoveSpeed = _char->GetMoveSpeed();
		float Defense = _char->GetDefense();
		float Prev = _char->GetPrevMoveSpeed();
		FPlayerInfo& Info = _char->GetInfo();

		if (Info.CurEP < m_arrEnergy[(int32)ESKILL::STIM_PACK])
			return;

		AttackSpeed *= 1.375f + (0.125f * _level);
		MoveSpeed *= 1.375f + (0.125f * _level);

		Defense -= Defense * (0.2875f + (0.0125f * _level));

		Info.CurEP -= m_arrEnergy[(int32)ESKILL::STIM_PACK];
		_char->SetAttackSpeed(AttackSpeed);
		_char->SetMoveSpeed(MoveSpeed);
		_char->SetDefense(Defense);

		_char->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		_char->GetAnimMontage()[0]->RateScale = AttackSpeed / 100.f;
	}
}

void USkillManager::Skill_DroneBomb(int32 _level, ACharacterBase* _char, TSubclassOf<ADrone> _drone)
{
	if (0 == _level)
		return;

	if (nullptr != _char && m_arrSkillUsable[(int32)ESKILL::DRONE_BOMB])
	{
		m_arrSkillUsable[(int32)ESKILL::DRONE_BOMB] = false;

		FPlayerInfo& Info = _char->GetInfo();

		if (Info.CurEP < m_arrEnergy[(int32)ESKILL::DRONE_BOMB])
			return;

		Info.CurEP -= m_arrEnergy[(int32)ESKILL::DRONE_BOMB];

		FActorSpawnParameters SpawnParam = {};
		SpawnParam.OverrideLevel = _char->GetLevel();
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParam.bDeferConstruction = true;

		FVector dir = _char->GetActorForwardVector();
		FVector pos = _char->GetActorLocation();
		pos -= _char->GetActorRightVector() * 100.f;
		pos.Z += 100.f;

		ADrone* drone = _char->GetWorld()->SpawnActor<ADrone>(_drone, FTransform(dir.Rotation(), pos), SpawnParam);
		drone->SetDroneDamage(_char->GetSkillDamage());
		drone->FinishSpawning(drone->GetTransform());
	}
}

void USkillManager::Skill_EnergyShield(int32 _level, ACharacterBase* _char)
{
	if (0 == _level)
		return;

	if (nullptr != _char && !m_bShield && m_arrSkillUsable[(int32)ESKILL::ENERGY_SHIELD])
	{
		m_bShield = true;
		m_arrSkillUsable[(int32)ESKILL::ENERGY_SHIELD] = false;

		FPlayerInfo& Info = _char->GetInfo();

		if (Info.CurEP < m_arrEnergy[(int32)ESKILL::ENERGY_SHIELD])
			return;

		Info.CurEP -= m_arrEnergy[(int32)ESKILL::ENERGY_SHIELD];

		Info.MaxShield = 250.f + (70.f * _level);
		Info.CurShield = Info.MaxShield;
	}
}

void USkillManager::Skill_SatelliteBeam(int32 _level, ACharacterBase* _char, TSubclassOf<AProjectile> _Beam, float _Start)
{
	if (0 == _level)
		return;

	if (nullptr != _char && m_arrSkillUsable[(int32)ESKILL::SATELLITE_BEAM])
	{
		m_arrSkillUsable[(int32)ESKILL::SATELLITE_BEAM] = false;

		FPlayerInfo& Info = _char->GetInfo();

		if (Info.CurEP < m_arrEnergy[(int32)ESKILL::SATELLITE_BEAM])
			return;

		Info.CurEP -= m_arrEnergy[(int32)ESKILL::SATELLITE_BEAM];

		FVector Pos = _char->GetActorLocation();
		FVector Foward = _char->GetActorForwardVector();

		FActorSpawnParameters SpawnParam = {};
		SpawnParam.OverrideLevel = _char->GetLevel();
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParam.bDeferConstruction = true;

		ASatelliteBeam* Beam = _char->GetWorld()->SpawnActor<ASatelliteBeam>(_Beam, Pos + Foward * _Start, Foward.Rotation(), SpawnParam);

		if (nullptr != Beam)
		{
			Beam->SetDamage(_char->GetSkillDamage());
			Beam->GetProjectileMovement()->Velocity = Foward;
			Beam->FinishSpawning(Beam->GetTransform());

			_char->SetBeam(Beam);
		}
	}
}

void USkillManager::Skill_CallGigant(int32 _level, ACharacterBase* _char, TSubclassOf<AGigant> _gigant)
{
	if (0 == _level)
		return;

	if (nullptr != _char && m_arrSkillUsable[(int32)ESKILL::CALL_GIGANT])
	{
		m_arrSkillUsable[(int32)ESKILL::CALL_GIGANT] = false;

		FPlayerInfo& Info = _char->GetInfo();

		if (Info.CurEP < m_arrEnergy[(int32)ESKILL::CALL_GIGANT])
			return;

		Info.CurEP -= m_arrEnergy[(int32)ESKILL::CALL_GIGANT];
		
		if (nullptr == m_Gigant)
		{
			FActorSpawnParameters SpawnParam = {};
			SpawnParam.OverrideLevel = _char->GetLevel();
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParam.bDeferConstruction = true;

			FVector dir = _char->GetActorForwardVector();
			FVector pos = _char->GetActorLocation();
			pos += dir * 1500.f;
			pos.Z += 1200.f;

			m_Gigant = _char->GetWorld()->SpawnActor<AGigant>(_gigant, FTransform(dir.Rotation(), pos), SpawnParam);

			m_Gigant->CalGigantDamage(_char->GetBaseDamage());
			m_Gigant->CalGigantHP(_level);
			m_Gigant->FinishSpawning(m_Gigant->GetTransform());
		}
		else
		{
			m_Gigant->FullRecovery();
		}
	}
}

void USkillManager::SkillCoolTime(float DeltaTime)
{
	for (int32 i = 0; i < (int32)ESKILL::END; ++i)
	{
		if (!m_arrSkillUsable[i])
		{
			m_arrCoolTimer[i] += DeltaTime;

			if (m_arrCoolTime[i] < m_arrCoolTimer[i])
			{
				m_arrCoolTimer[i] = 0.f;
				m_arrSkillUsable[i] = true;
			}
		}
	}
}

void USkillManager::StimPackTimeCheck(float DeltaTime, ACharacterBase* _char)
{
	m_StimPackTimer += DeltaTime;

	if (m_StimPackTime < m_StimPackTimer)
	{
		m_StimPackTimer = 0.f;
		m_bStimPack = false;

		if (nullptr != _char)
		{
			float OriginMoveSpeed = _char->GetPrevMoveSpeed();
			float OriginAttackSpeed = _char->GetPrevAttackSpeed();
			float OriginDefense = _char->GetPrevDefense();

			_char->SetAttackSpeed(OriginAttackSpeed);
			_char->SetMoveSpeed(OriginMoveSpeed);
			_char->SetDefense(OriginDefense);

			_char->GetCharacterMovement()->MaxWalkSpeed = OriginMoveSpeed;
			_char->GetAnimMontage()[0]->RateScale = OriginAttackSpeed / 100.f;
		}
	}
}

void USkillManager::ShieldTimeCheck(float DeltaTime, ACharacterBase* _char)
{
	m_ShieldTimer += DeltaTime;

	if (m_ShieldTime < m_ShieldTimer)
	{
		m_ShieldTimer = 0.f;
		m_bShield = false;

		if (nullptr != _char && !m_bShield)
		{
			FPlayerInfo& Info = _char->GetInfo();

			Info.MaxShield = 0.f;
			Info.CurShield = Info.MaxShield;
		}
	}
}

bool USkillManager::IsSkillUsed(ESKILL _skill)
{
	if (ESKILL::STIM_PACK == _skill)
	{
		return m_bStimPack;
	}
	else if (ESKILL::ENERGY_SHIELD == _skill)
	{
		return m_bShield;
	}

	return false;
}

void USkillManager::Reset()
{
	for (int i = 0; i < (int)ESKILL::END; ++i)
	{
		m_arrSkillLevel[i] = 0;
	}
}
