#include "MainGameInstance.h"

UMainGameInstance::UMainGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UMainGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
