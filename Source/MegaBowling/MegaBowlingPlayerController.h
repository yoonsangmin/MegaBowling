// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MegaBowlingPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class MEGABOWLING_API AMegaBowlingPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* SystemInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InGameInputMappingContext;

protected:

	virtual void BeginPlay() override;
};
