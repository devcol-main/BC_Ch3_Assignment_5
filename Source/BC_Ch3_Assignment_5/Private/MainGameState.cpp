#include "MainGameState.h"

AMainGameState::AMainGameState()
{
	Score = 0;
}

int32 AMainGameState::GetScore() const
{
	return Score;
}

void AMainGameState::AddScore(int32 Amount)
{
	Score += Amount;
}

