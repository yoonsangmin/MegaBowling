// Fill out your copyright notice in the Description page of Project Settings.


#include "MegaBowlingPlayer.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "BowlingBall.h"
#include "MegaBowlingGameMode.h"
#include "kismet/GameplayStatics.h"


// Sets default values
AMegaBowlingPlayer::AMegaBowlingPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CollisionComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = true;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	MovementComponent->UpdatedComponent = CollisionComponent;

	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AMegaBowlingPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerHeight = GetActorLocation().Z;
	MegaBowlingGameMode = Cast<AMegaBowlingGameMode>(UGameplayStatics::GetActorOfClass(GetWorld(), AMegaBowlingGameMode::StaticClass()));
}

// Called every frame
void AMegaBowlingPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	SetActorLocation(FVector(Location.X, Location.Y, PlayerHeight));
}

void AMegaBowlingPlayer::PositiveMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMegaBowlingPlayer::NegativeMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMegaBowlingPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X * LookInputScale);
		AddControllerPitchInput(LookAxisVector.Y * LookInputScale);
	}
}

void AMegaBowlingPlayer::Ready()
{
	// bIsShot - 발사 됐는데 아직 안 사라졌다
	// ObtainedBalls.Num() == 0 - 소유한 볼 초기화가 안 됨
	// BallCount <= 0 - 발사할 볼이 없음
	if (bIsShot || OwnedBalls.Num() == 0 || OwnedBalls[SelectedBallIndex].BallCount <= 0)
		return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = this;

	SpawnedBall = GetWorld()->SpawnActor<ABowlingBall>(OwnedBalls[SelectedBallIndex].BallClass, FTransform(GetActorRotation(), GetActorLocation() + GetActorForwardVector() * ShootOffSet, FVector::OneVector), SpawnInfo);
	SpawnedBall->Tags.Add("Spawned");
	if (SpawnedBall)
	{
		SpawnedBall->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}

	Guage = 0;
	bIsPowerIncreasing = true;

	OnReady();
}

void AMegaBowlingPlayer::Charge(const FInputActionInstance& Instance)
{
	// !SpawnedBall - 생성이 안 됐다
	// bIsShot - 발사 됐는데 아직 안 사라졌다
	if (!SpawnedBall || bIsShot)
		return;

	float DeltaSecond = GetWorld()->DeltaTimeSeconds;
	float TotalDeltaPower = MaxPower - MinPower;

	if (bIsPowerIncreasing)
	{
		Guage += DeltaSecond / PowerChargeDuration;
		if (Guage >= 1)
		{
			bIsPowerIncreasing = false;
			Guage = 2 - Guage;
		}
	}
	else
	{
		Guage -= DeltaSecond / PowerChargeDuration;
		if (Guage <= 0)
		{
			bIsPowerIncreasing = true;
			Guage = -Guage;
		}
	}

	Power = MinPower + Guage * TotalDeltaPower;

	OnCharging();
}

void AMegaBowlingPlayer::Shoot()
{
	// !SpawnedBall - 생성이 안 됐다
	// bIsShot - 발사 됐는데 아직 안 사라졌다
	if (!SpawnedBall || bIsShot)
		return;

	bIsShot = true;
	--OwnedBalls[SelectedBallIndex].BallCount;

	SpawnedBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SpawnedBall->ShootBall(
		GetActorForwardVector() * Power,
		[&] {
			SpawnedBall = nullptr;
			bIsShot = false;
		});
	
	OnShoot();
}

void AMegaBowlingPlayer::SelectPreviousBall()
{
	if (OwnedBalls.Num() == 0)
		return;

	// 볼을 들고 있는 경우
	if (SpawnedBall && !bIsShot)
		return;
	
	SelectedBallIndex = FMath::Max(0, --SelectedBallIndex);

	UpdateBallUI();
}

void AMegaBowlingPlayer::SelectNextBall()
{
	if (OwnedBalls.Num() == 0)
		return;

	// 볼을 들고 있는 경우
	if (SpawnedBall && !bIsShot)
		return;

	SelectedBallIndex = FMath::Min(OwnedBalls.Num() - 1, ++SelectedBallIndex);

	UpdateBallUI();
}

void AMegaBowlingPlayer::ResetStage()
{
	SpawnedBall = nullptr;
	bIsShot = false;

	MegaBowlingGameMode->ResetStage();
	UpdateBallUI();
}

void AMegaBowlingPlayer::PauseGame_Implementation()
{
	if (bIsGameStarted)
		bIsPaused = !bIsPaused;
}

void AMegaBowlingPlayer::OnStageStarted()
{
	SpawnedBall = nullptr;
	bIsShot = false;

	bIsPaused = false;
	SelectedBallIndex = 0;
	UpdateBallUI();
}

void AMegaBowlingPlayer::SetOwnedBalls(const TMap<int32, int32>& BallCountMap)
{
	if (!BallTable)
	{
		UE_LOG(LogTemp, Display, TEXT("Please Set Stage Data Table."));
		return;
	}

	OwnedBalls.Empty();
	DestroyAllBallSlotUI();

	int32 BallIndex = 0;
	for (const auto& Pair : BallCountMap)
	{
		if (auto BallDatum = BallTable->FindRow<FBallData>(FName(*FString::FromInt(Pair.Key)), ""))
		{
			if (BallDatum->BallClass.IsPending())
				BallDatum->BallClass.LoadSynchronous();
			OwnedBalls.Add(FBallClassCount(BallDatum->BallClass.Get(), Pair.Value));

			BallDatum->Thumbnail.LoadSynchronous();
			AddBallSlotUI(BallIndex++, BallDatum->Name, BallDatum->Thumbnail.Get(), Pair.Value);
		}
	}

	FinishToAddingBallSlotUI();
}

void AMegaBowlingPlayer::SetIsGameStarted(bool bIsStarted)
{
	bIsGameStarted = bIsStarted;
}

// Called to bind functionality to input
void AMegaBowlingPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(PositiveMoveAction, ETriggerEvent::Triggered, this, &AMegaBowlingPlayer::PositiveMove);
		EnhancedInputComponent->BindAction(NegativeMoveAction, ETriggerEvent::Triggered, this, &AMegaBowlingPlayer::NegativeMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMegaBowlingPlayer::Look);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMegaBowlingPlayer::Ready);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMegaBowlingPlayer::Charge);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AMegaBowlingPlayer::Shoot);
		EnhancedInputComponent->BindAction(PreviousBallAction, ETriggerEvent::Started, this, &AMegaBowlingPlayer::SelectPreviousBall);
		EnhancedInputComponent->BindAction(NextBallAction, ETriggerEvent::Started, this, &AMegaBowlingPlayer::SelectNextBall);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AMegaBowlingPlayer::PauseGame);
		EnhancedInputComponent->BindAction(ResetAction, ETriggerEvent::Triggered, this, &AMegaBowlingPlayer::ResetStage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}