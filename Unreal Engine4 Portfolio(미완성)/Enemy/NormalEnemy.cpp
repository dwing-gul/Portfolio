// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy.h"
#include "../UI/EnemyHPWidget.h"
#include "../Player/CharacterBase.h"

ANormalEnemy::ANormalEnemy() :
	m_Dist(200.f),
	m_AccTime(0.f),
	m_EndTime(5.f)
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Blueprint/Enemy/BT_NormalEnemy.BT_NormalEnemy'"));

	if (BT.Succeeded())
	{
		SetBehaviorTree(BT.Object);
	}

	ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/Blueprint/Enemy/BB_NormalEnemy.BB_NormalEnemy'"));

	if (BB.Succeeded())
	{
		SetBlackboard(BB.Object);
	}
}

ANormalEnemy::~ANormalEnemy()
{
}

void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalEnemy::Tick(float DeltaTime)
{
	if (EENEMY_STATE::DEAD == GetState())
	{
		m_AccTime += DeltaTime;

		if (m_EndTime < m_AccTime)
		{
			m_AccTime = 0.f;

			Destroy();
		}
	}

	Super::Tick(DeltaTime);

	HPVisible();
}

bool ANormalEnemy::ChangeState(EENEMY_STATE _NextState)
{
	if (EENEMY_STATE::HIT == _NextState)
	{
		SetStopable(false);
	}

	EENEMY_STATE PrevState = GetState();

	if (IsUnstopable() || EENEMY_STATE::DEAD == PrevState)
		return false;

	SetState(_NextState);

	EENEMY_STATE State = GetState();

	if (State == PrevState)
		return false;

	switch (State)
	{
	case EENEMY_STATE::IDLE:
		GetMovementComponent()->StopMovementImmediately();
		break;
	case EENEMY_STATE::MOVE:
		break;
	case EENEMY_STATE::ATTACK:
		SetStopable(true);
		break;
	case EENEMY_STATE::HIT:
		SetStopable(true);
		break;
	case EENEMY_STATE::DEAD:
		SetStopable(true);
		break;
	}

	return true;
}

void ANormalEnemy::HPVisible()
{
	UWidgetComponent* Widget = GetWidgetComponent();
	UEnemyHPWidget* HPBar = Cast<UEnemyHPWidget>(Widget->GetWidget());

	if (nullptr == HPBar)
		return;

	ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	if (nullptr == Player)
		return;

	FVector PlayerPos = Player->GetActorLocation();
	FVector Pos = GetActorLocation();

	float Dist = FVector::Distance(PlayerPos, Pos);

	if (Dist < m_Dist)
	{
		HPBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HPBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
