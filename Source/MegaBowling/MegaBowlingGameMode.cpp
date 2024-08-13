// Fill out your copyright notice in the Description page of Project Settings.


#include "MegaBowlingGameMode.h"
#include "MegaBowlingGameState.h"
#include "MegaBowlingPlayer.h"
#include "BowlingBall.h"
#include "BowlingPinComponent.h"
#include "StageData.h"
#include "BallData.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AMegaBowlingGameMode::AMegaBowlingGameMode()
{
	GameStateClass = AMegaBowlingGameState::StaticClass();
}

void AMegaBowlingGameMode::BeginPlay()
{
	Super::BeginPlay();

	Player = GetWorld()->GetFirstPlayerController()->GetPawn<AMegaBowlingPlayer>();
	MegaBowlingGameState = Cast<AMegaBowlingGameState>(UGameplayStatics::GetActorOfClass(GetWorld(), AMegaBowlingGameState::StaticClass()));

	if (Player)
		UE_LOG(LogTemp, Display, TEXT("Player Found"));
}

void AMegaBowlingGameMode::StartStage(int32 StageNumber)
{
	if (!StageTable)
	{
		UE_LOG(LogTemp, Display, TEXT("Please Set Stage Data Table."));
		return;
	}

	if (UWorld* World = GetWorld())
	{
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->ActorHasTag("Spawned"))
			{
				Actor->Destroy();
			}
		}
	}

	RemainedPin = 0;
	
	SelectedStage = StageNumber;

	if (auto StageDatum = StageTable->FindRow<FStageData>(FName(*FString::FromInt(SelectedStage)), ""))
	{
		for (const auto& ActorInfo : StageDatum->ActorInfos)
		{
			if (ActorInfo.ActorClass.IsPending())
				ActorInfo.ActorClass.LoadSynchronous();

			if (auto ActorClass = ActorInfo.ActorClass.Get())
			{
				FTransform SpawnTransform = ActorInfo.Transform;
				SpawnTransform.SetScale3D(FVector::One());
				AActor* SpawnedActor = GetWorld()->SpawnActor(ActorClass, &SpawnTransform);

				SpawnedActor->SetActorScale3D(ActorInfo.Transform.GetScale3D());
				SpawnedActor->Tags.Add(FName(TEXT("Spawned")));
				SpawnedActors.Add(SpawnedActor);
				
				if (auto BowlingPinComponent = SpawnedActor->GetComponentByClass<UBowlingPinComponent>())
				{
					++RemainedPin;
					
					BowlingPinComponent->AddOnDestroyCallback([&] {
						--RemainedPin;

						UE_LOG(LogTemp, Display, TEXT("[%s]: Remained Pin is %d."), *GetName(), RemainedPin);

						if (RemainedPin <= 0)
						{
							UE_LOG(LogTemp, Display, TEXT("[%s]: Stage is cleared."), *GetName());

							MegaBowlingGameState->AddClearedStages(SelectedStage);
							Player->OnStageCleared();
						}
					});
				}
			}
		}

		if (Player)
			Player->SetOwnedBalls(StageDatum->BallIDCountMap);

		MegaBowlingGameState->SetFallDeathHeight(StageDatum->FallDeathHeight);

		Player->SetIsGameStarted(true);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Stage Row Not Found"));
	}
}

void AMegaBowlingGameMode::ResetStage()
{
	StartStage(SelectedStage);
}