// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonBase.h"
#include "AI/SummonsAIController.h"
#include "../Enemy/Enemy.h"
#include "../UnrealProjectGameModeBase.h"
#include "../UI/MainHUDWidget.h"

// Sets default values
ASummonBase::ASummonBase() :
	m_State(ESUMMONS_STATE::SPAWN),
	m_AccTime(0.f),
	m_EndTime(2.f),
	m_bDead(false),
	m_bUnstopable(false),
	m_PrevHP(0.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASummonsAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SummonType"));
}

ASummonBase::~ASummonBase()
{
	
}

// Called when the game starts or when spawned
void ASummonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASummonBase::Tick(float DeltaTime)
{
	if (ESUMMONS_STATE::DEAD == m_State)
	{
		m_AccTime += DeltaTime;

		if (m_EndTime < m_AccTime)
		{
			m_AccTime = 0.f;
			m_bDead = true;
		}

		if (m_bDead)
		{
			AUnrealProjectGameModeBase* GM = Cast<AUnrealProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			UMainHUDWidget* MainHUD = GM->GetMainHUD();
			MainHUD->GetSummonsHUD()->SetVisibility(ESlateVisibility::Hidden);

			Destroy();
		}

		return;
	}

	Super::Tick(DeltaTime);

	if (m_PrevHP != m_Info.CurHP)
	{
		AUnrealProjectGameModeBase* GM = Cast<AUnrealProjectGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		float HPRatio = m_Info.CurHP / m_Info.MaxHP;

		GM->UpdateSummonHUD(HPRatio);
	}

	m_PrevHP = m_Info.CurHP;

	if (m_Info.CurHP <= 0.f)
	{
		Death();
	}
}

// Called to bind functionality to input
void ASummonBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ASummonBase::ChangeState(ESUMMONS_STATE _NextState)
{
	if (ESUMMONS_STATE::HIT == _NextState)
	{
		m_bUnstopable = false;
	}

	if (m_bUnstopable == true || m_State == _NextState || ESUMMONS_STATE::DEAD == m_State)
		return false;

	m_State = _NextState;

	switch (m_State)
	{
	case ESUMMONS_STATE::IDLE:
		GetMovementComponent()->StopMovementImmediately();
		break;
	case ESUMMONS_STATE::MOVE:
		break;
	case ESUMMONS_STATE::ATTACK:
		m_bUnstopable = true;
		break;
	case ESUMMONS_STATE::HIT:
		m_bUnstopable = true;
		break;
	case ESUMMONS_STATE::DEAD:
		m_bUnstopable = true;
		break;
	case ESUMMONS_STATE::SPAWN:
		m_bUnstopable = true;
		break;
	}

	return true;
}

void ASummonBase::OnAttackTrigger()
{
	FVector Pos = GetActorLocation();
	FVector Foward = GetActorForwardVector();
	Foward.Z = 0.f;
	Foward.Normalize();

	TArray<FHitResult> arrHit;
	FCollisionQueryParams param(NAME_None, false, this);
	float Range = m_Info.AttackRange;
	float Degree = 90.f;

	GetWorld()->SweepMultiByChannel(arrHit, Pos, Pos, FQuat::Identity, ECC_GameTraceChannel3, FCollisionShape::MakeSphere(Range), param);

	if (arrHit.Num())
	{
		for (size_t i = 0; i < arrHit.Num(); ++i)
		{
			FVector TargetDir = arrHit[i].Actor->GetActorLocation() - Pos;
			TargetDir.Z = 0.f;
			TargetDir.Normalize();

			float Dot = FVector::DotProduct(TargetDir, Foward);
			float Radian = FMath::Acos(Dot);
			float Angle = FMath::Abs(FMath::RadiansToDegrees(Radian));

			if (Angle < Degree / 2.f)
			{
				FTransform Trans(arrHit[i].ImpactNormal.Rotation(), arrHit[i].ImpactPoint);
				/*UEffectManager::GetInst(GetWorld())->CreateEffect(EEFFECT_TYPE::EXPLOSION, Trans, GetLevel());*/
				AEnemy* Enemy = Cast<AEnemy>(arrHit[i].Actor);

				if(nullptr != Enemy)
				{
					Enemy->ChangeState(EENEMY_STATE::HIT);
					Enemy->GetMovementComponent()->StopMovementImmediately();
					Enemy->Hit(m_Info.AttackPower);
				}
			}
		}
	}

#ifdef ENABLE_DRAW_DEBUG
	FColor color = arrHit.Num() != 0 ? FColor::Red : FColor::Green;
	DrawDebugSphere(GetWorld(), Pos, Range, 20, color, false, 2.5f);
#endif
}

void ASummonBase::Hit(float _damage)
{
	m_Info.CurHP -= _damage;

	if (m_Info.CurHP <= 0.f)
	{
		m_Info.CurHP = 0.f;
	}
}

void ASummonBase::Death()
{
	ChangeState(ESUMMONS_STATE::DEAD);
}

