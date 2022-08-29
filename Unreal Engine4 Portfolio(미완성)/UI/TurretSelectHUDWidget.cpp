// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretSelectHUDWidget.h"
#include <Components/Image.h>

void UTurretSelectHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_SelectType = ESTRUCTURE_TYPE::NONE;

	m_BarrierBP = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("Blueprint'/Game/Blueprint/Structure/BPC_Barrier.BPC_Barrier_C'"));
	m_Turret_ABP = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("Blueprint'/Game/Blueprint/Structure/BPC_Turret_A.BPC_Turret_A_C'"));
	m_Turret_BBP = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("Blueprint'/Game/Blueprint/Structure/BPC_Turret_B.BPC_Turret_B_C'"));
	m_Turret_CBP = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("Blueprint'/Game/Blueprint/Structure/BPC_Turret_C.BPC_Turret_C_C'"));
	m_Turret_DBP = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("Blueprint'/Game/Blueprint/Structure/BPC_Turret_D.BPC_Turret_D_C'"));

	m_Barrier = Cast<UButton>(GetWidgetFromName(TEXT("Barrier")));
	m_Turret_A = Cast<UButton>(GetWidgetFromName(TEXT("Turret_A")));
	m_Turret_B = Cast<UButton>(GetWidgetFromName(TEXT("Turret_B")));
	m_Turret_C = Cast<UButton>(GetWidgetFromName(TEXT("Turret_C")));
	m_Turret_D = Cast<UButton>(GetWidgetFromName(TEXT("Turret_D")));
	m_None = Cast<UButton>(GetWidgetFromName(TEXT("NoStructure")));

	m_Current = Cast<UTextBlock>(GetWidgetFromName(TEXT("Current")));

	m_StructurePos1 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos1")));
	m_StructurePos2 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos2")));
	m_StructurePos3 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos3")));
	m_StructurePos4 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos4")));
	m_StructurePos5 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos5")));
	m_StructurePos6 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos6")));
	m_StructurePos7 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos7")));
	m_StructurePos8 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos8")));
	m_StructurePos9 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos9")));
	m_StructurePos10 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos10")));
	m_StructurePos11 = Cast<UButton>(GetWidgetFromName(TEXT("StructurePos11")));

	m_Barrier->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::BarrierOnClicked);
	m_Turret_A->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::Turret_AOnClicked);
	m_Turret_B->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::Turret_BOnClicked);
	m_Turret_C->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::Turret_COnClicked);
	m_Turret_D->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::Turret_DOnClicked);
	m_None->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::NoneOnClicked);

	m_StructurePos1->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos1OnClicked);
	m_StructurePos2->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos2OnClicked);
	m_StructurePos3->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos3OnClicked);
	m_StructurePos4->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos4OnClicked);
	m_StructurePos5->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos5OnClicked);
	m_StructurePos6->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos6OnClicked);
	m_StructurePos7->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos7OnClicked);
	m_StructurePos8->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos8OnClicked);
	m_StructurePos9->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos9OnClicked);
	m_StructurePos10->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos10OnClicked);
	m_StructurePos11->OnClicked.AddDynamic(this, &UTurretSelectHUDWidget::StructurePos11OnClicked);
}

void UTurretSelectHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTurretSelectHUDWidget::BarrierOnClicked()
{
	m_SelectType = ESTRUCTURE_TYPE::BARRIER;

	if (nullptr != m_Current)
	{
		m_Current->SetText(FText::FromString(TEXT("Current : Barrier")));
	}
	else
	{
		m_Current = Cast<UTextBlock>(GetWidgetFromName(TEXT("Current")));
		m_Current->SetText(FText::FromString(TEXT("Current : Barrier")));
	}
}

void UTurretSelectHUDWidget::Turret_AOnClicked()
{
	m_SelectType = ESTRUCTURE_TYPE::TURRET_A;

	if (nullptr != m_Current)
	{
		m_Current->SetText(FText::FromString(TEXT("Current : Turret A")));
	}
	else
	{
		m_Current = Cast<UTextBlock>(GetWidgetFromName(TEXT("Current")));
		m_Current->SetText(FText::FromString(TEXT("Current : Turret A")));
	}
}

void UTurretSelectHUDWidget::Turret_BOnClicked()
{
	m_SelectType = ESTRUCTURE_TYPE::TURRET_B;

	if (nullptr != m_Current)
	{
		m_Current->SetText(FText::FromString(TEXT("Current : Turret B")));
	}
	else
	{
		m_Current = Cast<UTextBlock>(GetWidgetFromName(TEXT("Current")));
		m_Current->SetText(FText::FromString(TEXT("Current : Turret B")));
	}
}

void UTurretSelectHUDWidget::Turret_COnClicked()
{
	m_SelectType = ESTRUCTURE_TYPE::TURRET_C;

	if (nullptr != m_Current)
	{
		m_Current->SetText(FText::FromString(TEXT("Current : Turret C")));
	}
	else
	{
		m_Current = Cast<UTextBlock>(GetWidgetFromName(TEXT("Current")));
		m_Current->SetText(FText::FromString(TEXT("Current : Turret C")));
	}
}

void UTurretSelectHUDWidget::Turret_DOnClicked()
{
	m_SelectType = ESTRUCTURE_TYPE::TURRET_D;

	if (nullptr != m_Current)
	{
		m_Current->SetText(FText::FromString(TEXT("Current : Turret D")));
	}
	else
	{
		m_Current = Cast<UTextBlock>(GetWidgetFromName(TEXT("Current")));
		m_Current->SetText(FText::FromString(TEXT("Current : Turret D")));
	}
}

void UTurretSelectHUDWidget::NoneOnClicked()
{
	m_SelectType = ESTRUCTURE_TYPE::NONE;

	if (nullptr != m_Current)
	{
		m_Current->SetText(FText::FromString(TEXT("Current : None")));
	}
	else
	{
		m_Current = Cast<UTextBlock>(GetWidgetFromName(TEXT("Current")));
		m_Current->SetText(FText::FromString(TEXT("Current : None")));
	}
}

void UTurretSelectHUDWidget::StructurePos1OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos1);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP1)
		{
			m_SP1->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos1"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP1 = SettingStructure(m_SP1, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos2OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos2);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP2)
		{
			m_SP2->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos2"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP2 = SettingStructure(m_SP2, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos3OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos3);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP3)
		{
			m_SP3->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos3"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP3 = SettingStructure(m_SP3, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos4OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos4);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP4)
		{
			m_SP4->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos4"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP4 = SettingStructure(m_SP4, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos5OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos5);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP5)
		{
			m_SP5->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos5"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP5 = SettingStructure(m_SP5, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos6OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos6);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP6)
		{
			m_SP6->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos6"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP6 = SettingStructure(m_SP6, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos7OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos7);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP7)
		{
			m_SP7->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos7"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP7 = SettingStructure(m_SP7, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos8OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos8);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP8)
		{
			m_SP8->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos8"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP8 = SettingStructure(m_SP8, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos9OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos9);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP9)
		{
			m_SP9->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos9"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP9 = SettingStructure(m_SP9, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos10OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos10);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP10)
		{
			m_SP10->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos10"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP10 = SettingStructure(m_SP10, SettingPos);
	}
}

void UTurretSelectHUDWidget::StructurePos11OnClicked()
{
	SelectImage(m_SelectType, m_StructurePos11);

	if (ESTRUCTURE_TYPE::NONE == m_SelectType)
	{
		if (nullptr != m_SP11)
		{
			m_SP11->Destroy();
		}
	}
	else
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
		AActor* SettingPos = nullptr;

		for (int32 i = 0; i < Actors.Num(); ++i)
		{
			if (Actors[i]->GetName() == TEXT("StructurePos11"))
			{
				SettingPos = Actors[i];
				break;
			}
		}

		m_SP11 = SettingStructure(m_SP11, SettingPos);
	}
}

void UTurretSelectHUDWidget::SelectImage(ESTRUCTURE_TYPE _SelectType, UButton* _Widget)
{
	UTexture2D* RenderTarget = nullptr;

	switch (m_SelectType)
	{
	case ESTRUCTURE_TYPE::NONE:
		RenderTarget = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Engine/EngineResources/Black.Black'"));
		break;
	case ESTRUCTURE_TYPE::BARRIER:
		RenderTarget = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Barrier_Tex.Barrier_Tex'"));
		break;
	case ESTRUCTURE_TYPE::TURRET_A:
		RenderTarget = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Turret_A_Tex.Turret_A_Tex'"));
		break;
	case ESTRUCTURE_TYPE::TURRET_B:
		RenderTarget = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Turret_B_Tex.Turret_B_Tex'"));
		break;
	case ESTRUCTURE_TYPE::TURRET_C:
		RenderTarget = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Turret_C_Tex.Turret_C_Tex'"));
		break;
	case ESTRUCTURE_TYPE::TURRET_D:
		RenderTarget = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Image/RenderTarget/Turret_D_Tex.Turret_D_Tex'"));
		break;
	}

	if (nullptr != RenderTarget)
	{
		UImage* Select = Cast<UImage>(_Widget->GetChildAt(0));
		Select->SetBrushFromTexture(RenderTarget);
	}
}

AStructureBase* UTurretSelectHUDWidget::SettingStructure(AStructureBase* _SP, AActor* _Setting)
{
	FActorSpawnParameters SpawnParam = {};
	SpawnParam.OverrideLevel = _Setting->GetLevel();
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.bDeferConstruction = true;

	FVector Pos = _Setting->GetActorLocation();

	switch (m_SelectType)
	{
	case ESTRUCTURE_TYPE::BARRIER:
	{
		Pos.Z = 3231.631592f;

		if (nullptr == _SP)
		{
			_SP = _Setting->GetWorld()->SpawnActor<ABarrier>(m_BarrierBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(FMath::Sqrt(7.f), FMath::Sqrt(7.f), FMath::Sqrt(7.f)));

			_SP->FinishSpawning(_SP->GetTransform());

		}
		else
		{
			_SP->Destroy();
			_SP = _Setting->GetWorld()->SpawnActor<ABarrier>(m_BarrierBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(FMath::Sqrt(7.f), FMath::Sqrt(7.f), FMath::Sqrt(7.f)));

			_SP->FinishSpawning(_SP->GetTransform());
		}
	}
	break;
	case ESTRUCTURE_TYPE::TURRET_A:
	{
		Pos.Z = 3394.652832f;

		if (nullptr == _SP)
		{
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_ABP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(FMath::Sqrt(4.5f), FMath::Sqrt(4.5f), FMath::Sqrt(4.5f)));


			_SP->FinishSpawning(_SP->GetTransform());

		}
		else
		{
			_SP->Destroy();
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_ABP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(FMath::Sqrt(4.5f), FMath::Sqrt(4.5f), FMath::Sqrt(4.5f)));

			_SP->FinishSpawning(_SP->GetTransform());
		}
	}
	break;
	case ESTRUCTURE_TYPE::TURRET_B:
	{
		Pos.Z = 3293.941406f;

		if (nullptr == _SP)
		{
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_BBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(FMath::Sqrt(3.5f), FMath::Sqrt(3.5f), FMath::Sqrt(3.5f)));

			_SP->FinishSpawning(_SP->GetTransform());

		}
		else
		{
			_SP->Destroy();
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_BBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(FMath::Sqrt(3.5f), FMath::Sqrt(3.5f), FMath::Sqrt(3.5f)));

			_SP->FinishSpawning(_SP->GetTransform());
		}
	}
	break;
	case ESTRUCTURE_TYPE::TURRET_C:
	{
		Pos.Z = 3425.11377;

		if (nullptr == _SP)
		{
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_CBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(2.f, 2.f, 2.f));

			_SP->FinishSpawning(_SP->GetTransform());

		}
		else
		{
			_SP->Destroy();
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_CBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(2.f, 2.f, 2.f));

			_SP->FinishSpawning(_SP->GetTransform());
		}
	}
	break;
	case ESTRUCTURE_TYPE::TURRET_D:
	{
		Pos.Z = 3264.313721f;

		if (nullptr == _SP)
		{
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_DBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(2.f, 2.f, 2.f));

			_SP->FinishSpawning(_SP->GetTransform());

		}
		else
		{
			_SP->Destroy();
			_SP = _Setting->GetWorld()->SpawnActor<ATurret>(m_Turret_DBP, Pos, _Setting->GetActorRotation(), SpawnParam);
			_SP->SetActorScale3D(FVector(2.f, 2.f, 2.f));

			_SP->FinishSpawning(_SP->GetTransform());
		}
	}
	break;
	}

	return _SP;
}
