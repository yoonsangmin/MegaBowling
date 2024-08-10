// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MegaBowlingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MEGABOWLING_API UMegaBowlingSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)

	TArray<int32> ClearedStages;
};
