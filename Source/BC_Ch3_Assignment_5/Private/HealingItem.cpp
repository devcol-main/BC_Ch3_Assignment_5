#include "HealingItem.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 회복 디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
			//FString::Printf(TEXT("Player Gained %d HP!"), HealAmount));
			FString::Printf(TEXT("Player Gained %i HP!"), HealAmount));
        
		DestroyItem();
	}
}