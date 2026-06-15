#include "DebuffItem.h"
#include "MainCharacter.h"

ADebuffItem::ADebuffItem()
{
	DebuffData.DebuffType = EDebuffType::None;
	DebuffData.Duration = 0.0f;
	DebuffData.Amount = 0.0f;
}

void ADebuffItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,FString::Printf(TEXT("Player Gained %i HP!"), HealAmount));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
		                                 FString::Printf(TEXT("Activate DebuffItem")));

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
		FString::Printf(
		TEXT("DebuffName: %s"),
		*UEnum::GetValueAsString(DebuffData.DebuffType)));

		if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(Activator))
		{
			//
			if (DebuffData.DebuffType == EDebuffType::Slow)
			{
				// 약한 포인터
				TWeakObjectPtr<AMainCharacter> WeakPlayer = PlayerCharacter;
				float CurrentSpeed = PlayerCharacter->GetSpeed();
				
				PlayerCharacter->DebuffSpeed(CurrentSpeed * 0.5f, DebuffData.Duration);				
			}
			else if (DebuffData.DebuffType == EDebuffType::ReverseControl)
			{
				PlayerCharacter->DebuffReverseControl(DebuffData.Duration);
			}
			
			
		}

		DestroyItem();
	}
}
