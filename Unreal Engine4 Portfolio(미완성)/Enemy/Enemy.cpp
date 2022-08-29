// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../Projectile/Projectile.h"
#include "AI/EnemyAIController.h"
#include "../Player/CharacterBase.h"
#include "../Summons/SummonBase.h"
#include "../Structure/StructureBase.h"

// Sets default values
AEnemy::AEnemy() :
	m_State(EENEMY_STATE::IDLE),
	m_Gold(5),
	m_AccTime(0.f),
	m_EndTime(2.f),
	m_PrevHP(m_Info.CurHP),
	m_PrevEP(m_Info.CurEP),
	m_bDead(false),
	m_bUnstopable(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	m_WidgetComponent->SetupAttachment(GetMesh());

	AIControllerClass = AEnemyAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyType"));
}

AEnemy::~AEnemy()
{
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	if (EENEMY_STATE::DEAD == m_State)
	{
		m_AccTime += DeltaTime;

		if (m_EndTime < m_AccTime)
		{
			m_AccTime = 0.f;
			m_bDead = true;
		}

		if (m_bDead)
		{
			Destroy();
		}

		return;
	}

	Super::Tick(DeltaTime);

	if (m_PrevHP != m_Info.CurHP)
	{
		float Ratio = m_Info.CurHP / m_Info.MaxHP;

	}

	if (m_PrevEP != m_Info.CurEP)
	{
		float Ratio = m_Info.CurEP / m_Info.MaxEP;

	}

	m_PrevHP = m_Info.CurHP;
	m_PrevEP = m_Info.CurEP;

	if (m_Info.CurHP < 0.f)
	{
		Death();
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Hit(float _damage)
{
	ChangeState(EENEMY_STATE::HIT);

	m_Info.CurHP -= _damage;

	if (m_Info.CurHP <= 0.f)
	{
		ChangeState(EENEMY_STATE::DEAD);
	}
}

void AEnemy::Death()
{
	SetState(EENEMY_STATE::DEAD);

	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	if (nullptr == Player)
		return;

	Player->AddGold(m_Gold);
}

void AEnemy::OnAttackTrigger()
{
	FVector Pos = GetActorLocation();
	FVector Foward = GetActorForwardVector();
	Foward.Z = 0.f;
	Foward.Normalize();

	TArray<FHitResult> arrHit;
	FCollisionQueryParams param(NAME_None, false, this);
	float Range = m_Info.AttackRange;
	float Degree = 90.f;

	GetWorld()->SweepMultiByChannel(arrHit, Pos, Pos, FQuat::Identity, ECC_GameTraceChannel4, FCollisionShape::MakeSphere(Range), param);

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
				ACharacterBase* Player = Cast<ACharacterBase>(arrHit[i].Actor);
				ASummonBase* Summon = Cast<ASummonBase>(arrHit[i].Actor);
				AStructureBase* Structure = Cast<AStructureBase>(arrHit[i].Actor);

				if (nullptr != Player)
				{
					Player->ChangeState(EPLAYER_STATE::HIT);
					Player->GetMovementComponent()->StopMovementImmediately();
					Player->Hit(m_Info.AttackPower);
					break;
				}

				if (nullptr != Summon)
				{
					Summon->ChangeState(ESUMMONS_STATE::HIT);
					Summon->GetMovementComponent()->StopMovementImmediately();
					Summon->Hit(m_Info.AttackPower);
					break;
				}

				if (nullptr != Structure)
				{
					Structure->Hit(m_Info.AttackPower);
					break;
				}
			}
		}
	}

//#ifdef ENABLE_DRAW_DEBUG
//	FColor color = arrHit.Num() != 0 ? FColor::Red : FColor::Green;
//	DrawDebugSphere(GetWorld(), Pos, Range, 20, color, false, 2.5f);
//#endif
}

void AEnemy::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	LOG(Error, TEXT("EnemyOverlap"));

	if (ECC_GameTraceChannel9 == _OtherComp->GetCollisionObjectType())
	{
		AProjectile* Projectile = Cast<AProjectile>(_OtherComp->GetOwner());

		if (nullptr != Projectile)
		{
			float Damage = Projectile->GetDamage();
			Hit(Damage);
			GetMovementComponent()->StopMovementImmediately();
		}
	}
}
