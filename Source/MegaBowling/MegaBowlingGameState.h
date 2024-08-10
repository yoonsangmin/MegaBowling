// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MegaBowlingGameState.generated.h"

/**
 * 
 */
UCLASS()
class MEGABOWLING_API AMegaBowlingGameState : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
	float FallDeathHeight = 0.0f;

public:
	UFUNCTION(BlueprintCallable)
	void LoadSaveGame();

	UFUNCTION(BlueprintCallable)
	float GetFallDeathHeight();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<int32> GetClearedStages();

	void SetFallDeathHeight(float Height);
	void AddClearedStages(int32 StageNumber);

private:
	TArray<int32> ClearedStages;
};
