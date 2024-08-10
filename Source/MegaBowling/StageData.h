#pragma once

#include "CoreMinimal.h"
#include "StageData.generated.h"


USTRUCT(BlueprintType)
struct FActorStruct
{
	GENERATED_BODY()

	// Shoot
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform Transform;
};

USTRUCT(BlueprintType)
struct MEGABOWLING_API FStageData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FActorStruct> ActorInfos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, int32> BallIDCountMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FallDeathHeight;
};
