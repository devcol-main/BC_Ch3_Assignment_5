#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"


UCLASS()
class BC_CH3_ASSIGNMENT_5_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();
	
	// 폭발까지 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionDelay;
	// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionRadius;
	// 폭발 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionDamage;

	virtual void ActivateItem(AActor* Activator) override;
};
