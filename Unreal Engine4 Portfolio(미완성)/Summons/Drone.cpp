// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "../Player/CharacterBase.h"
#include "../Enemy/Enemy.h"
#include "../Manager/EffectManager.h"

// Sets default values
ADrone::ADrone() :
	m_Speed(100.f),
	m_ExplosionRadius(200.f),
	m_Damage(10.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneMesh(TEXT("SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica_Drone/Meshes/Belica_Drone.Belica_Drone'"));

	if (DroneMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DroneMesh.Object);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BombType"));

	m_Dest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	m_Dest->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (SphereMesh.Succeeded())
	{
		m_Dest->SetStaticMesh(SphereMesh.Object);
	}

	m_Dest->SetRelativeLocation(FVector(100.f, 0.f, -46.63f));
	m_Dest->SetVisibility(false);
}

ADrone::~ADrone()
{
	
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADrone::OnBeginOverlap);
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Pos = GetActorLocation();
	FVector Dir = m_Dest->GetComponentLocation() - Pos;
	Dir.Normalize();

	Pos += Dir * DeltaTime * m_Speed;

	SetActorLocation(Pos);
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADrone::SetDroneDamage(float _damage)
{
	m_Damage = 2.f * _damage;
}

void ADrone::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (ECC_GameTraceChannel2 == _OtherComp->GetCollisionObjectType() || ECC_GameTraceChannel6 == _OtherComp->GetCollisionObjectType())
	{
		// 몬스터 혹은 바닥과 충돌 후 터지면서 구 모양의 데미지 섹션 생성
		//폭발 파티클 생성
		// 구형 체크
		FVector Pos = GetActorLocation();
		FVector Foward = GetActorForwardVector();
		Foward.Z = 0.f;
		Foward.Normalize();

		TArray<FHitResult> arrHit;
		FCollisionQueryParams param(NAME_None, false, this);

		GetWorld()->SweepMultiByChannel(arrHit, Pos, Pos, FQuat::Identity, ECC_GameTraceChannel3, FCollisionShape::MakeSphere(m_ExplosionRadius), param);

		if (arrHit.Num())
		{
			for (size_t i = 0; i < arrHit.Num(); ++i)
			{
				FTransform Trans(arrHit[i].ImpactNormal.Rotation(), arrHit[i].ImpactPoint);
				UEffectManager::GetInst(GetWorld())->CreateEffect(EEFFECT_TYPE::DRONE_EXPLOSION, Trans, GetLevel());
				USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("SoundCue'/Game/Blueprint/Sound/SQ_Drone_Explosion.SQ_Drone_Explosion'"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
				AEnemy* Enemy = Cast<AEnemy>(arrHit[i].Actor);

				if(nullptr != Enemy)
				{
					Enemy->ChangeState(EENEMY_STATE::HIT);
					Enemy->GetMovementComponent()->StopMovementImmediately();
					Enemy->Hit(m_Damage);
				}
			}
		}

#ifdef ENABLE_DRAW_DEBUG
		FColor color = arrHit.Num() != 0 ? FColor::Red : FColor::Green;
		DrawDebugSphere(GetWorld(), Pos, m_ExplosionRadius, 20, color, false, 2.5f);
#endif

		Destroy();
	}
}