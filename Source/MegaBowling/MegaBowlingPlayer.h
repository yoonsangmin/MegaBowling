// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallData.h"
#include "MegaBowlingPlayer.generated.h"

class UInputAction;
struct FInputActionValue;
struct FInputActionInstance;
class USphereComponent;
class UPawnMovementComponent;
class UCameraComponent;
class ABowlingBall;
class AMegaBowlingGameMode;

UCLASS()
class MEGABOWLING_API AMegaBowlingPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMegaBowlingPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnMovementComponent> MovementComponent;

protected:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PositiveMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> NegativeMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float LookInputScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PreviousBallAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> NextBallAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ResetAction;

	// Game Play
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PlayerHeight = 32.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGameStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPaused = false;

	// Shoot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stage", meta = (RequiredAssetDataTags = "RowStructure=/Script/MegaBowling.BallData"))
	TObjectPtr<UDataTable> BallTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	TArray<FBallClassCount> OwnedBalls;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	int32 SelectedBallIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	float ShootOffSet = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	float MinPower = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	float MaxPower = 500000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	float PowerChargeDuration = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Shoot")
	float Guage;

	// Input
	void PositiveMove(const FInputActionValue& Value);
	void NegativeMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Ready();
	void Charge(const FInputActionInstance& Instance);
	void Shoot();
	void SelectPreviousBall();
	void SelectNextBall();
	void ResetStage();

	// Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void OnReady();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharging();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DestroyAllBallSlotUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateBallUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddBallSlotUI(int32 BallIndex, const FText& Name, const UTexture2D* Textrue, int32 Count);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FinishToAddingBallSlotUI();
	
	UFUNCTION(BlueprintNativeEvent)
	void PauseGame();
	void PauseGame_Implementation();

public:
	// Game Play
	UFUNCTION(BlueprintCallable)
	void OnStageStarted();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnStageCleared();

	void SetOwnedBalls(const TMap<int32, int32>& BallMap);
	void SetIsGameStarted(bool bIsStarted);

private:
	float Power;
	bool bIsPowerIncreasing;
	bool bIsShot = false;
	TObjectPtr<ABowlingBall> SpawnedBall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AMegaBowlingGameMode> MegaBowlingGameMode;
};
