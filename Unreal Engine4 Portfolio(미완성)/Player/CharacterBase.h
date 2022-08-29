// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "../ProjectStruct.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UPlayerAnimInstance;
class ADrone;
class AGigant;
class AProjectile;

UCLASS()
class UNREALPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo m_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> m_Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rot, meta = (AllowPrivateAccess = "true"))
	float m_RotSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rot, meta = (AllowPrivateAccess = "true"))
	float m_RotUpperLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rot, meta = (AllowPrivateAccess = "true"))
	float m_RotLowerLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackCheck, meta = (AllowPrivateAccess = "true"))
	FName m_RayStartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackCheck, meta = (AllowPrivateAccess = "true"))
	float m_AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameResource, meta = (AllowPrivateAccess = "true"))
	int32 m_Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameResource, meta = (AllowPrivateAccess = "true"))
	int32 m_Point;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADrone> m_Drone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGigant> m_Gigant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float m_BeamStart;

	UPlayerAnimInstance* m_AnimInst;
	AProjectile* m_Beam;

	ESKILL m_arrSkill[(int32)ESKILL_KEY::END];

	float m_PrevHP;
	float m_PrevEP;
	float m_PrevShield;
	float m_HPRecovery;
	float m_EPRecovery;

protected :
	float m_MoveSpeed;
	float m_AttackSpeed;
	float m_BaseDamage;
	float m_SkillDamage;
	float m_Defense;

	float m_PrevMoveSpeed;
	float m_PrevAttackSpeed;
	float m_PrevDefense;

	TArray<UAnimMontage*> m_arrMontage;

	bool m_bClicked;

public:
	// Sets default values for this character's properties
	ACharacterBase();
	~ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Attack() {}
	virtual void HitEffect(FTransform _trans) {}

public :
	void ChangeState(EPLAYER_STATE _NextState);
	void CalStat();
	void SkillSetting(ESKILL_KEY _key, ESKILL _skill);

	EPLAYER_STATE GetState() { return m_State; }
	FPlayerInfo& GetInfo() { return m_Info; }

	UPlayerAnimInstance* GetAnimInst() { return m_AnimInst; }
	TArray<UAnimMontage*> GetAnimMontage() { return m_arrMontage; }

	int32 GetGold() { return m_Gold; }
	int32 GetPoint() { return m_Point; }

	void AddGold(int32 _gold) { m_Gold += _gold; }
	void SubGold(int32 _gold) { m_Gold -= _gold; }
	bool CanUseGold(int32 _gold) { return 0 <= (m_Gold - _gold); }

	void AddPoint(int32 _point) { m_Point += _point; }
	void SubPoint(int32 _point) { m_Point -= _point; }
	bool CanUsePoint(int32 _point) { return 0 <= (m_Point - _point); }

	void SetMoveSpeed(float _speed) { m_PrevMoveSpeed = m_MoveSpeed; m_MoveSpeed = _speed; }
	float GetMoveSpeed() { return m_MoveSpeed; }
	float GetPrevMoveSpeed() { return m_PrevMoveSpeed; }

	void SetAttackSpeed(float _speed) { m_PrevAttackSpeed = m_AttackSpeed; m_AttackSpeed = _speed; }
	float GetAttackSpeed() { return m_AttackSpeed; }
	float GetPrevAttackSpeed() { return m_PrevAttackSpeed; }

	void SetDefense(float _defense) { m_PrevDefense = m_Defense; m_Defense = _defense; }
	float GetDefense() { return m_Defense; }
	float GetPrevDefense() { return m_PrevDefense; }

	void SetBaseDamage(float _damage) { m_BaseDamage = _damage; }
	float GetBaseDamage() { return m_BaseDamage; }

	void SetSkillDamage(float _damage) { m_SkillDamage = _damage; }
	float GetSkillDamage() { return m_SkillDamage; }

	void SetBeam(AProjectile* _Beam) { m_Beam = _Beam; }

public :
	void MoveFront(float _Scale);
	void MoveSide(float _Scale);
	void RotationZ(float _Scale);
	void RotationY(float _Scale);

	void Skill_Q();
	void Skill_E();
	void Skill_Ultimate();

	virtual void Fire();
	virtual void Hit(float _damage, EDAMAGE_TYPE _type = EDAMAGE_TYPE::BASE);
	virtual void AttackTrigger();

	void Death();

private :
	void FireEnd();
};
