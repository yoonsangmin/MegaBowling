// Fill out your copyright notice in the Description page of Project Settings.


#include "MegaBowlingGameState.h"
#include "MegaBowlingSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AMegaBowlingGameState::LoadSaveGame()
{
	if (UMegaBowlingSaveGame* LoadedGame = Cast<UMegaBowlingSaveGame>(UGameplayStatics::LoadGameFromSlot("0", 0)))
	{
		ClearedStages = LoadedGame->ClearedStages;
		UE_LOG(LogTemp, Display, TEXT("[%s]: Game is loaded."), *GetName());
	}
}

float AMegaBowlingGameState::GetFallDeathHeight()
{
	return FallDeathHeight;
}

void AMegaBowlingGameState::SetFallDeathHeight(float Height)
{
	FallDeathHeight = Height;
}

void AMegaBowlingGameState::AddClearedStages(int32 StageNumber)
{
	ClearedStages.Add(StageNumber);

	if (UMegaBowlingSaveGame* SaveGameInstance = Cast<UMegaBowlingSaveGame>(UGameplayStatics::CreateSaveGameObject(UMegaBowlingSaveGame::StaticClass())))
	{
		 SaveGameInstance->ClearedStages = ClearedStages;

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "0", 0))
		{
			UE_LOG(LogTemp, Display, TEXT("[%s]: Game is saved."), *GetName());
		}
	}
}

TArray<int32> AMegaBowlingGameState::GetClearedStages()
{
	return ClearedStages;
}
