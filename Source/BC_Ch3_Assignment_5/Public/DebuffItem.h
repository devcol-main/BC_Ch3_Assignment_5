#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "DebuffItem.generated.h"


UENUM(BlueprintType)
enum class EDebuffType : uint8
{
	None UMETA(DisplayName = "None"),
	Slow UMETA(DisplayName = "Slow"),
	ReverseControl UMETA(DisplayName = "ReverseControl"),
	Blind UMETA(DisplayName = "Blind")
};

USTRUCT(BlueprintType)
struct FDebuffData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	EDebuffType DebuffType = EDebuffType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float Duration = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float Amount = 0.0f;
	

	FDebuffData()
	{
		DebuffType = EDebuffType::None;
		Duration = 0.0f;	
		Amount = 0.0f;
	}
};

UCLASS()
class BC_CH3_ASSIGNMENT_5_API ADebuffItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ADebuffItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	FDebuffData DebuffData;	
	
	virtual void ActivateItem(AActor* Activator) override;	

protected:
	//FTimerHandle DebuffTimerHandle;
	
};