// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPinComponent.h"
#include "MegaBowlingGameState.h"


// Sets default values for this component's properties
UBowlingPinComponent::UBowlingPinComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	UpCheckerClass = USceneComponent::StaticClass();
}


// Called when the game starts
void UBowlingPinComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!UpCheckerClass)
		return;

	AActor* Owner = GetOwner();

	FTransform UpCheckerTransform;
	UpCheckerTransform.SetLocation(Owner->GetActorLocation() + FVector(0, 0, UpCheckerZOffset));
	UpCheckerTransform.SetRotation(FRotator(90, 0, 0).Quaternion());
	
	UpChecker = Cast<USceneComponent>(Owner->AddComponentByClass(UpCheckerClass, true, UpCheckerTransform, false));
	UpChecker->SetHiddenInGame(bHiddenUpChecker);
	UpChecker->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	
	Owner->AddInstanceComponent(UpChecker);

	GameState = GetWorld()->GetGameState<AMegaBowlingGameState>();
}


// Called every frame
void UBowlingPinComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckPin();
}

void UBowlingPinComponent::AddOnDestroyCallback(const TFunction<void ()> &DestroyCallback)
{
	OnDestroyCallback = DestroyCallback;
}

void UBowlingPinComponent::CheckPin()
{
	AActor* Owner = GetOwner();
	float Dot = FVector::DotProduct(UpChecker->GetForwardVector(), FVector(0, 0, 1));
	bool bIsKnockedDown = Dot < KnockDownThreshold && Owner->GetVelocity().Length() < StabilizingSpeed;
	bool bIsFalling = Owner->GetActorLocation().Z < GameState->GetFallDeathHeight();
	if (bIsKnockedDown || bIsFalling)
	{
		if (!DestructionHandle.IsValid())
		{
			if (bIsKnockedDown)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s]: Dot is %f."), *Owner->GetName(), Dot);
				UE_LOG(LogTemp, Display, TEXT("[%s]: Bowling Pin is knocked down. Timer is triggered"), *Owner->GetName());
			}
			if (bIsFalling)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s]: Bowling Pin is Fallen. Timer is triggered"), *Owner->GetName());
			}
			
			GetWorld()->GetTimerManager().SetTimer(DestructionHandle, this, &UBowlingPinComponent::DestroyPin, KnockDownTime);
		}
	}
	else if (DestructionHandle.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s]: Bowling Pin is recovered. Timer is removed"), *Owner->GetName());
		GetWorld()->GetTimerManager().ClearTimer(DestructionHandle);
	}
}

void UBowlingPinComponent::DestroyPin()
{
	if (OnDestroyCallback)
		OnDestroyCallback();
	GetOwner()->Destroy();
}