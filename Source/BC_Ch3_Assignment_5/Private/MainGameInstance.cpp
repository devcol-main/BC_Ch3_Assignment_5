#include "MainGameInstance.h"

UMainGameInstance::UMainGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UMainGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	//(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
