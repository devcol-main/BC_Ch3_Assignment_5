#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"


UCLASS()
class BC_CH3_ASSIGNMENT_5_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ACoinItem();
	
protected:	
	// 코인 획득 시 얻을 점수 (자식 클래스에서 상속받아 값만 바꿔줄 예정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
};
