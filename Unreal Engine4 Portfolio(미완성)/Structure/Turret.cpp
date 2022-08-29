// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "../Enemy/Enemy.h"
#include "AI/TurretAIController.h"
#include "../Manager/StructureManager.h"
#include "../Manager/EffectManager.h"

ATurret::ATurret() :
	m_State(ETURRET_STATE::IDLE),
	m_Target(nullptr),
	m_ReloadTime(3.f),
	m_RemainTime(1.5f),
	m_TurnSpeed(2.f),
	m_AccTime(m_ReloadTime),
	m_DestroyTimer(0.f),
	m_Head(nullptr)
{
	SetStaticMeshComponent(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model")));
	GetStaticMeshComponent()->SetupAttachment(GetBox());

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Blueprint/Structure/BT_Turret.BT_Turret'"));

	if (BT.Succeeded())
	{
		m_BehaviorTree = BT.Object;
	}

	ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/Blueprint/Structure/BB_Turret.BB_Turret'"));

	if (BB.Succeeded())
	{
		m_Blackboard = BB.Object;
	}

	AIControllerClass = ATurretAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetBox()->SetCollisionProfileName(TEXT("StructureType"));
}

ATurret::~ATurret()
{
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	m_MaxLevel = UStructureManager::GetInst(GetWorld())->GetMaxLv();
	m_TurnSpeed = UStructureManager::GetInst(GetWorld())->GetTurnSpeed(GetType());

	USceneComponent* Child = GetStaticMeshComponent()->GetChildComponent(0);

	while (Child->GetName() != TEXT("Head"))
	{
		Child = Child->GetChildComponent(0);

		if (nullptr == Child)
			break;
	}

	if (Child->GetName() == TEXT("Head"))
	{
		m_Head = Child;
	}
}

void ATurret::Tick(float DeltaTime)
{
	if (ETURRET_STATE::DESTROY == m_State)
	{
		m_DestroyTimer += DeltaTime;

		if (m_RemainTime < m_DestroyTimer)
		{
			Destroy();
			return;
		}
	}

	Super::Tick(DeltaTime);

	m_Level = UStructureManager::GetInst(GetWorld())->GetCurLv(GetType());

	if (ETURRET_STATE::IDLE == m_State)
	{
		HeadSpin(DeltaTime);
	}
	else if (ETURRET_STATE::ATTACK == m_State)
	{
		Attack(DeltaTime);
	} 
}

void ATurret::ChangeState(ETURRET_STATE _NextState)
{
	if (ETURRET_STATE::DESTROY == m_State || m_State == _NextState)
		return;

	m_State = _NextState;
}

void ATurret::Attack(float _DeltaTime)
{
	if (nullptr == m_Target)
		return;

	//머리가 적 방향으로 회전
	FVector Pos = GetActorLocation();
	FVector Foward = GetActorForwardVector();
	FVector TargetPos = m_Target->GetActorLocation();
	FRotator Rot = m_Head->GetRelativeRotation();
	FVector Dir = TargetPos - Pos;
	Dir.Z = 0.f;
	Dir.Normalize();

	float Cos = FVector::DotProduct(Foward, Dir);
	float Radian = FMath::Acos(Cos);
	float Degree = FMath::RadiansToDegrees(Radian);

	FVector Up = GetActorUpVector();
	FVector Cross = FVector::CrossProduct(Foward, Dir);
	bool bTurn = false;

	if (Cross.Z > 0.f)
	{
		if (Rot.Yaw - Degree > 0.01f)
			Rot.Yaw += Degree * m_TurnSpeed * _DeltaTime;
		else
			bTurn = true;
	}
	else
	{
		if (Rot.Yaw + Degree > 0.01f)
			Rot.Yaw += -Degree * m_TurnSpeed * _DeltaTime;
		else
			bTurn = true;
	}
	
	m_Head->SetRelativeRotation(Rot);

	// 일정 시간 간격으로 히트스캔
	if (bTurn)
	{
		m_AccTime += _DeltaTime;

		if (m_ReloadTime < m_AccTime)
		{
			m_AccTime = 0.f;

			TArray<FHitResult> arrHit;
			FCollisionQueryParams param(NAME_None, false, this);

			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("SoundCue'/Game/Blueprint/Sound/SQ_Turret_Fire.SQ_Turret_Fire'"));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());

			FVector start = m_Head->GetComponentLocation();
			start.Z += 50.f;
			FVector foward = m_Head->GetForwardVector();
			foward.Normalize();
			FVector end = start + foward * GetInfo().AttackRange;

			bool bHit = GetWorld()->LineTraceMultiByChannel(arrHit, start, end, ECC_GameTraceChannel3, param);

			if (arrHit.Num())
			{
				for (size_t i = 0; i < arrHit.Num(); ++i)
				{
					FTransform Trans(arrHit[i].ImpactNormal.Rotation(), arrHit[i].ImpactPoint);
					UEffectManager::GetInst(GetWorld())->CreateEffect(EEFFECT_TYPE::HIT_BY_TURRET, Trans, GetLevel());
					Sound = LoadObject<USoundBase>(nullptr, TEXT("SoundCue'/Game/Blueprint/Sound/SQ_Turret_AtkPoint.SQ_Turret_AtkPoint'"));
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
					AEnemy* Enemy = Cast<AEnemy>(arrHit[i].Actor);

					if (nullptr != Enemy)
					{
						Enemy->ChangeState(EENEMY_STATE::HIT);
						Enemy->GetMovementComponent()->StopMovementImmediately();
						Enemy->Hit(GetInfo().AttackPower);
					}
				}
			}

//#ifdef ENABLE_DRAW_DEBUG
//			FColor color = arrHit.Num() != 0 ? FColor::Red : FColor::Green;
//			DrawDebugLine(GetWorld(), start, end, color, false, 20.f, 0, 2.f);
//#endif // DEBUG
		}
	}
}

void ATurret::HeadSpin(float _DeltaTime)
{
	FRotator Rot = m_Head->GetRelativeRotation();
	Rot.Yaw += _DeltaTime * 90.f;
	m_Head->SetRelativeRotation(Rot);
}

void ATurret::Destruction()
{
	FTransform Trans(GetActorRotation(), GetActorLocation(), GetActorScale3D());
	UEffectManager::GetInst(GetWorld())->CreateEffect(EEFFECT_TYPE::DESTROY_TURRET, Trans, GetLevel());
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("SoundCue'/Game/Blueprint/Sound/SQ_Turret_Explosion.SQ_Turret_Explosion'"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
	ChangeState(ETURRET_STATE::DESTROY);
}
