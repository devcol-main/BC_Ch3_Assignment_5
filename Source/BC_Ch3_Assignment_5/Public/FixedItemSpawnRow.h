#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase 정의가 들어있는 헤더
#include "FixedItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FFixedItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass; // 원래는 소프트 레퍼런스인 TSoftClassPtr 로 하는게 맞습니다.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveIndex;
	
	// 이 아이템의 스폰 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};
