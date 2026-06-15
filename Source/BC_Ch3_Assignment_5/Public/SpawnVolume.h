#pragma once

#include "CoreMinimal.h"
#include "FixedItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"       // 우리가 정의한 구조체

#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class BC_CH3_ASSIGNMENT_5_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	
	UFUNCTION(BlueprintCallable, Category = "Spawning")	
	AActor* SpawnRandomItem(); 
	
	UFUNCTION(BlueprintCallable, Category = "FixedSpawning")	
	AActor* SpawnFixedItem(); 
	
	FFixedItemSpawnRow* GetFixedItem() const;
	
	void SpawnFixedItemsForWave(int32 WaveIndex);
	FFixedItemSpawnRow* GetFixedItemByWave(int32 WaveIndex) const;
	
protected:
	//
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FixedSpawning")
	UDataTable* FixedItemDataTable;
	
	
	
	//
	FItemSpawnRow* GetRandomItem() const;
	
	// === Components	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
    USceneComponent* Scene;
    // 스폰 영역을 담당할 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
    UBoxComponent* SpawningBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;
	
	// === Functions
	
	// 특정 아이템 클래스를 스폰하는 함수
	UFUNCTION(BlueprintCallable, Category="Spawning")
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass); // 리턴 형식을 AActor* 로 변경
	
	// 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
	UFUNCTION(BlueprintCallable, Category="Spawning")
	FVector GetRandomPointInVolume() const;	
	
	
};
