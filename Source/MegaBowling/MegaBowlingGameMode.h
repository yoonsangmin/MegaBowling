// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MegaBowlingGameMode.generated.h"


UCLASS()
class MEGABOWLING_API AMegaBowlingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMegaBowlingGameMode();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartStage(int32 StageNumber);

	UFUNCTION(BlueprintCallable)
	void ResetStage();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stage", meta = (RequiredAssetDataTags = "RowStructure=/Script/MegaBowling.StageData"))
	TObjectPtr<UDataTable> StageTable;

	UPROPERTY(BlueprintReadOnly, Category = "Stage")
	int32 SelectedStage;

private:
	TObjectPtr<class AMegaBowlingPlayer> Player;
	TObjectPtr<class AMegaBowlingGameState> MegaBowlingGameState;
	TArray<TObjectPtr<AActor>> SpawnedActors;
	int32 RemainedPin;
};
