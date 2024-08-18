// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BowlingBall.generated.h"

class UProjectileMovementComponent;
class AMegaBowlingGameState;

UCLASS()
class MEGABOWLING_API ABowlingBall : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABowlingBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void ShootBall(const FVector& Impulse, const TFunction<void ()> &FinishCallback);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	TArray<UMaterialInterface*> TranslucentMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DestroyDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StabilizingSpeed = 50.0f;

	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DestroyBall();
	virtual void DestroyBall_Implementation();

	UFUNCTION(BlueprintCallable)
	void FinishDestoryBall();

private:
	// 부모 변수 셰도우 - 가리고 캐싱
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	TArray<UMaterialInterface*> OriginalMaterials;
	bool bIsShot = false;
	TFunction<void()> OnDestoryCallback;
	FTimerHandle DestructionHandle;
	TObjectPtr<AMegaBowlingGameState> GameState;

	void CheckDestroy();
};
