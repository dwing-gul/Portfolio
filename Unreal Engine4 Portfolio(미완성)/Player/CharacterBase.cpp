// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "../UnrealProjectGameModeBase.h"
#include "PlayerAnimInstance.h"
#include "../Manager/SkillManager.h"
#include "../Summons/Drone.h"
#include "../Summons/Gigant.h"
#include "../Projectile/Projectile.h"
#include "../Enemy/Enemy.h"

// Sets default values
ACharacterBase::ACharacterBase() :
	m_State(EPLAYER_STATE::IDLE),
	m_RotSpeed(180.f),
	m_RotUpperLimit(20.f),
	m_RotLowerLimit(-25.f),
	m_RayStartSocketName(TEXT("Muzzle_01")),
	m_AttackRange(1000.f),
	m_Gold(500),
	m_Point(10),
	m_BeamStart(300.f),
	m_AnimInst(nullptr),
	m_Beam(nullptr),
	m_PrevHP(100.f),
	m_PrevEP(50.f),
	m_PrevShield(0.f),
	m_HPRecovery(0.f),
	m_EPRecovery(1.f),
	m_MoveSpeed(600.f),
	m_AttackSpeed(100.f),
	m_BaseDamage(10.f),
	m_SkillDamage(15.f),
	m_Defense(5.f),
	m_PrevMoveSpeed(m_MoveSpeed),
	m_PrevAttackSpeed(m_AttackSpeed),
	m_PrevDefense(m_Defense),
	m_bClicked(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	m_Arm->SetupAttachment(GetMesh());
	m_Camera->SetupAttachment(m_Arm);

	for (int32 i = 0; i < (int32)ESKILL_KEY::END; ++i)
	{
		m_arrSkill[i] = ESKILL::END;
	}

	m_arrSkill[(int32)ESKILL_KEY::Q] = ESKILL::STIM_PACK;
	m_arrSkill[(int32)ESKILL_KEY::R] = ESKILL::CALL_GIGANT;
}

ACharacterBase::~ACharacterBase()
{
	
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	m_Info.CurHP = m_Info.MaxHP;
	m_Info.CurEP = m_Info.MaxEP;

	AUnrealProjectGameModeBase* GM = Cast<AUnrealProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->UpdateQSkillHUD();
	GM->UpdateESkillHUD();
	GM->UpdateRSkillHUD();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != m_Beam && m_Beam->IsActorBeingDestroyed())
	{
		m_Beam = nullptr;
	}

	if (USkillManager::GetInst(GetWorld())->IsSkillUsed(ESKILL::STIM_PACK))
	{
		USkillManager::GetInst(GetWorld())->StimPackTimeCheck(DeltaTime, this);
	}

	if (USkillManager::GetInst(GetWorld())->IsSkillUsed(ESKILL::ENERGY_SHIELD))
	{
		USkillManager::GetInst(GetWorld())->ShieldTimeCheck(DeltaTime, this);
	}

	USkillManager::GetInst(GetWorld())->SkillCoolTime(DeltaTime);

	if (m_PrevHP != m_Info.CurHP || m_PrevEP != m_Info.CurEP)
	{
		AUnrealProjectGameModeBase* GM = Cast<AUnrealProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		float HPRatio = m_Info.CurHP / m_Info.MaxHP;
		float EPRatio = m_Info.CurEP / m_Info.MaxEP;
		float ShieldRatio = m_Info.CurShield / m_Info.MaxShield;

		GM->UpdatePlayerHUD(HPRatio, EPRatio, ShieldRatio);
	}

	m_PrevHP = m_Info.CurHP;
	m_PrevEP = m_Info.CurEP;
	m_PrevShield = m_Info.CurShield;

	if (m_Info.CurHP < m_Info.MaxHP)
	{
		m_Info.CurHP += m_HPRecovery * DeltaTime;
	}
	else
	{
		m_Info.CurHP = m_Info.MaxHP;
	}

	if (m_Info.CurEP < m_Info.MaxEP)
	{
		m_Info.CurEP += m_EPRecovery * DeltaTime;
	}
	else
	{
		m_Info.CurEP = m_Info.MaxEP;
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (EPLAYER_STATE::DEAD != m_State)
	{
		PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &ACharacterBase::MoveFront);
		PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &ACharacterBase::MoveSide);
		PlayerInputComponent->BindAxis(TEXT("RotationZ"), this, &ACharacterBase::RotationZ);
		PlayerInputComponent->BindAxis(TEXT("RotationY"), this, &ACharacterBase::RotationY);

		PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ACharacterBase::Fire);
		PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &ACharacterBase::FireEnd);

		PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &ACharacterBase::Skill_Q);
		PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &ACharacterBase::Skill_E);
		PlayerInputComponent->BindAction(TEXT("Skill_Ultimate"), EInputEvent::IE_Pressed, this, &ACharacterBase::Skill_Ultimate);
	}

}

void ACharacterBase::ChangeState(EPLAYER_STATE _NextState)
{
	if (m_State == _NextState || EPLAYER_STATE::DEAD == m_State)
		return;

	m_State = _NextState;

	if (EPLAYER_STATE::ATTACK == m_State)
	{
		Attack();
	}
}

void ACharacterBase::CalStat()
{
	// 임시 공식 현재 스탯 계산은 수정해야됨
	m_Info.MaxHP += m_Info.MaxHP * (m_Info.Health / 100.f);
	m_Info.MaxEP += m_Info.MaxEP * (m_Info.Mental / 100.f);

	m_BaseDamage = m_Info.Power * 1.25f;
	m_SkillDamage = m_BaseDamage + m_Info.Intellect;
	m_MoveSpeed = m_Info.Speed;
	m_AttackSpeed = m_Info.Speed * 2.f;
	m_Defense = m_Info.Health;
}

void ACharacterBase::SkillSetting(ESKILL_KEY _key, ESKILL _skill)
{
	if (ESKILL::NONE == _skill)
	{
		USkillManager::GetInst(GetWorld())->SetSkillIcon(_key, _skill);
		m_arrSkill[(int32)_key] = _skill;
		return;
	}

	switch (_key)
	{
	case ESKILL_KEY::Q:
	case ESKILL_KEY::E:
	{
		if ((int32)ESKILL::NORMAL_SKILL < (int32)_skill && (int32)_skill < (int32)ESKILL::ULTIMATE_SKILL)
		{
			USkillManager::GetInst(GetWorld())->SetSkillIcon(_key, _skill);
			m_arrSkill[(int32)_key] = _skill;
		}
	}
		break;
	case ESKILL_KEY::R:
	{
		if ((int32)ESKILL::ULTIMATE_SKILL < (int32)_skill && (int32)_skill < (int32)ESKILL::END)
		{
			USkillManager::GetInst(GetWorld())->SetSkillIcon(_key, _skill);
			m_arrSkill[(int32)_key] = _skill;
		}
	}
		break;
	}
}

void ACharacterBase::MoveFront(float _Scale)
{
	if (EPLAYER_STATE::IDLE == m_State || EPLAYER_STATE::MOVE == m_State)
	{
		if (0.f == _Scale)
		{
			ChangeState(EPLAYER_STATE::IDLE);
		}
		else
		{
			ChangeState(EPLAYER_STATE::MOVE);
		}
	}

	AddMovementInput(GetActorForwardVector(), _Scale);
	
	float dir = m_AnimInst->GetDirection();

	if (1.f == _Scale)
	{
		if (90.f == dir)
		{
			m_AnimInst->SetDirection(45.f);
		}
		else if (0.f == dir)
		{
			m_AnimInst->SetDirection(0.f);
		}
		else if (-90.f == dir)
		{
			m_AnimInst->SetDirection(-45.f);
		}
	}
	else if (-1.f == _Scale)
	{
		if (90.f == dir)
		{
			m_AnimInst->SetDirection(135.f);
		}
		else if (0.f == dir)
		{
			m_AnimInst->SetDirection(180.f);
		}
		else if (-90.f == dir)
		{
			m_AnimInst->SetDirection(-135.f);
		}
	}
}

void ACharacterBase::MoveSide(float _Scale)
{
	if (EPLAYER_STATE::IDLE == m_State || EPLAYER_STATE::MOVE == m_State)
	{
		if (0.f == _Scale)
		{
			ChangeState(EPLAYER_STATE::IDLE);
		}
		else
		{
			ChangeState(EPLAYER_STATE::MOVE);
		}
	}

	AddMovementInput(GetActorRightVector(), _Scale);

	float dir = m_AnimInst->GetDirection();

	if (1.f == _Scale)
	{
		m_AnimInst->SetDirection(90.f);
	}
	else if (-1.f == _Scale)
	{
		m_AnimInst->SetDirection(-90.f);
	}
	else
	{
		m_AnimInst->SetDirection(0.f);
	}
}

void ACharacterBase::RotationZ(float _Scale)
{
	AddControllerYawInput(_Scale);
}

void ACharacterBase::RotationY(float _Scale)
{
	/*AddControllerPitchInput(_Scale);

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator Rot = m_Arm->GetRelativeRotation();

	Rot.Pitch += DeltaTime * _Scale * m_RotSpeed;

	if (m_RotUpperLimit < Rot.Pitch)
	{
		Rot.Pitch = m_RotUpperLimit;
	}
	else if (Rot.Pitch < m_RotLowerLimit)
	{
		Rot.Pitch = m_RotLowerLimit;
	}

	m_Arm->SetRelativeRotation(Rot);*/
}

void ACharacterBase::Skill_Q()
{
	bool bUsable = USkillManager::GetInst(GetWorld())->IsSkillUsable(m_arrSkill[(int32)ESKILL_KEY::Q]);

	if (!bUsable)
		return;

	int32 Level = USkillManager::GetInst(GetWorld())->GetSkillLevel(m_arrSkill[(int32)ESKILL_KEY::Q]);

	switch (m_arrSkill[(int32)ESKILL_KEY::Q])
	{
	case ESKILL::STIM_PACK:
		USkillManager::GetInst(GetWorld())->Skill_StimPack(Level, this);
		break;
	case ESKILL::DRONE_BOMB:
		USkillManager::GetInst(GetWorld())->Skill_DroneBomb(Level, this, m_Drone);
		break;
	case ESKILL::ENERGY_SHIELD:
		USkillManager::GetInst(GetWorld())->Skill_EnergyShield(Level, this);
		break;
	}
}

void ACharacterBase::Skill_E()
{
	bool bUsable = USkillManager::GetInst(GetWorld())->IsSkillUsable(m_arrSkill[(int32)ESKILL_KEY::E]);

	if (!bUsable)
		return;

	int32 Level = USkillManager::GetInst(GetWorld())->GetSkillLevel(m_arrSkill[(int32)ESKILL_KEY::E]);

	switch (m_arrSkill[(int32)ESKILL_KEY::E])
	{
	case ESKILL::STIM_PACK:
		USkillManager::GetInst(GetWorld())->Skill_StimPack(Level, this);
		break;
	case ESKILL::DRONE_BOMB:
		USkillManager::GetInst(GetWorld())->Skill_DroneBomb(Level, this, m_Drone);
		break;
	case ESKILL::ENERGY_SHIELD:
		USkillManager::GetInst(GetWorld())->Skill_EnergyShield(Level, this);
		break;
	}
}

void ACharacterBase::Skill_Ultimate()
{
	bool bUsable = USkillManager::GetInst(GetWorld())->IsSkillUsable(m_arrSkill[(int32)ESKILL_KEY::R]);

	if (!bUsable)
		return;

	int32 Level = USkillManager::GetInst(GetWorld())->GetSkillLevel(m_arrSkill[(int32)ESKILL_KEY::R]);

	switch (m_arrSkill[(int32)ESKILL_KEY::R])
	{
	case ESKILL::SATELLITE_BEAM:
		USkillManager::GetInst(GetWorld())->Skill_SatelliteBeam(Level, this, m_Projectile, m_BeamStart);
		break;
	case ESKILL::CALL_GIGANT:
		USkillManager::GetInst(GetWorld())->Skill_CallGigant(Level, this, m_Gigant);
		break;
	}
}

void ACharacterBase::Fire()
{
	m_bClicked = true;
	ChangeState(EPLAYER_STATE::ATTACK);
}

void ACharacterBase::Hit(float _damage, EDAMAGE_TYPE _type)
{
	ChangeState(EPLAYER_STATE::HIT);

	float Damage = _damage - m_Defense;

	if (Damage <= 0)
	{
		Damage = 1.f;
	}

	if (m_Info.CurShield - Damage < 0.f)
	{
		float Remain = m_Info.CurShield - Damage;
		m_Info.CurShield -= Damage;

		if (m_Info.CurShield < 0.f)
			m_Info.CurShield = 0.f;

		m_Info.CurHP -= Remain;

		if (m_Info.CurHP <= 0.f)
		{
			Death();
		}
	}
}

void ACharacterBase::Death()
{
	ChangeState(EPLAYER_STATE::DEAD);
}

void ACharacterBase::AttackTrigger()
{
	TArray<FHitResult> arrHit;
	FCollisionQueryParams param(NAME_None, false, this);

	FVector start = GetMesh()->GetSocketLocation(m_RayStartSocketName);
	FVector foward = GetActorForwardVector();
	foward.Normalize();
	FVector end = start + foward * m_AttackRange;

	bool bHit = GetWorld()->LineTraceMultiByChannel(arrHit, start, end, ECC_GameTraceChannel3, param);

	if (arrHit.Num())
	{
		for (size_t i = 0; i < arrHit.Num(); ++i)
		{
			FTransform Trans(arrHit[i].ImpactNormal.Rotation(), arrHit[i].ImpactPoint);
			HitEffect(Trans);
			AEnemy* Enemy = Cast<AEnemy>(arrHit[i].Actor);

			if (nullptr != Enemy)
			{
				Enemy->ChangeState(EENEMY_STATE::HIT);
				Enemy->GetMovementComponent()->StopMovementImmediately();
				Enemy->Hit(m_BaseDamage);
			}
		}
	}

//#ifdef ENABLE_DRAW_DEBUG
//	FColor color = arrHit.Num() != 0 ? FColor::Red : FColor::Green;
//	DrawDebugLine(GetWorld(), start, end, color, false, 20.f, 0, 2.f);
//#endif // DEBUG
}

void ACharacterBase::FireEnd()
{
	m_bClicked = false;
	Attack();
}