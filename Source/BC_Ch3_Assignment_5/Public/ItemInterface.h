#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};


class BC_CH3_ASSIGNMENT_5_API IItemInterface
{
	GENERATED_BODY()

	
public:
	// 구현할 아이템 종류 지뢰 , 힐링, 코인
	// 힐링, 코인: 즉시 발동형 | 오버랩
	// 지뢰: 5초뒤 발동 | 범위 내에 오버랩 
	
	
	// 플레이어가 이 아이템의 범위에 들어왔을 때 호출
	virtual void OnItemOverlap(AActor* OverlapActor) = 0;
	// 플레이어가 이 아이템의 범위를 벗어났을 때 호출
	virtual void OnItemEndOverlap(AActor* OverlapActor) = 0;
	// 아이템이 사용되었을 때 호출
	virtual void ActivateItem(AActor* Activator) = 0;
	// 이 아이템의 유형(타입)을 반환 (예: "Coin", "Mine" 등)
	virtual FName GetItemType() const = 0;
};
