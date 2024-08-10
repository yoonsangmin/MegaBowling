#pragma once

#include "CoreMinimal.h"
#include "BallData.generated.h"


USTRUCT(BlueprintType)
struct FBallClassCount
{
	GENERATED_BODY()

	// Shoot
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABowlingBall> BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BallCount = 3;
};

USTRUCT(BlueprintType)
struct MEGABOWLING_API FBallData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class ABowlingBall> BallClass;
};
