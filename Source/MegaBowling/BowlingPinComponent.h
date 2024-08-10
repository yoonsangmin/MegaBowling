// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BowlingPinComponent.generated.h"

class AMegaBowlingGameState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABOWLING_API UBowlingPinComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBowlingPinComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddOnDestroyCallback(const TFunction<void ()> &DestroyCallback);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpChecker")
	TSubclassOf<USceneComponent> UpCheckerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpChecker")
	float UpCheckerZOffset = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpChecker")
	bool bHiddenUpChecker = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpChecker")
	float KnockDownThreshold = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpChecker")
	float KnockDownTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UpChecker")
	float StabilizingSpeed = 25.0f;

private:
	TObjectPtr<USceneComponent> UpChecker;
	FTimerHandle DestructionHandle;
	TObjectPtr<AMegaBowlingGameState> GameState;
	TFunction<void ()> OnDestroyCallback;

	void CheckPin();
	void DestroyPin();
};
