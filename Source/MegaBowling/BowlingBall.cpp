// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingBall.h"
#include "MegaBowlingGameState.h"


// Sets default values
ABowlingBall::ABowlingBall()
{
 //	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
	StaticMeshComponent->SetUseCCD(true);
}

// Called when the game starts or when spawned
void ABowlingBall::BeginPlay()
{
	Super::BeginPlay();
	
	GameState = GetWorld()->GetGameState<AMegaBowlingGameState>();

	OriginalMaterials = StaticMeshComponent->GetMaterials();
	for (int32 i = 0; i < TranslucentMaterials.Num(); ++i)
	{
		StaticMeshComponent->SetMaterial(i, TranslucentMaterials[i]);
	}
}

// Called every frame
void ABowlingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsShot)
		CheckDestroy();
}

void ABowlingBall::ShootBall(const FVector& Impulse, const TFunction<void ()> &FinishCallback)
{
	OnDestoryCallback = FinishCallback;

	for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
	{
		StaticMeshComponent->SetMaterial(i, OriginalMaterials[i]);
	}

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->AddImpulse(Impulse, NAME_None, false);
	bIsShot = true;

	OnShoot();
}

void ABowlingBall::CheckDestroy()
{
	bool bIsStabilized = GetVelocity().Length() < StabilizingSpeed;
	bool bIsFalling = GetActorLocation().Z < GameState->GetFallDeathHeight();
	if (bIsStabilized || bIsFalling)
	{
		if (!DestructionHandle.IsValid())
		{
			if (bIsStabilized)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s]: Bowling Ball is stabilized. Timer is triggered"), *GetName());
			}
			if (bIsFalling)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s]: Bowling Ball is Fallen. Timer is triggered"), *GetName());
			}

			GetWorldTimerManager().SetTimer(DestructionHandle, this, &ABowlingBall::DestroyBall, DestroyDelay);
		}
	}
	else if (DestructionHandle.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s]: Bowling Ball is still moving. Timer is removed"), *GetName());
		GetWorld()->GetTimerManager().ClearTimer(DestructionHandle);
	}
}

void ABowlingBall::DestroyBall_Implementation()
{
	FinishDestoryBall();
}

void ABowlingBall::FinishDestoryBall()
{
	if (OnDestoryCallback)
		OnDestoryCallback();
	Destroy();
}
