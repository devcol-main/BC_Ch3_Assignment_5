#include "HealingItem.h"
#include "MainCharacter.h"

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
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,FString::Printf(TEXT("Player Gained %i HP!"), HealAmount));
		if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(Activator))
		{
			// 캐릭터의 체력을 회복
			PlayerCharacter->AddHealth(HealAmount);
		}
			
		
		DestroyItem();
	}
}