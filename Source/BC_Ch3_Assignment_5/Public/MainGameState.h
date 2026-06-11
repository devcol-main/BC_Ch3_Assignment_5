#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"


UCLASS()
class BC_CH3_ASSIGNMENT_5_API AMainGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMainGameState();	
	
	// === Variables ===
	// 전역 점수를 저장하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score")
	int32 Score;

	
	// === Func ===
	// 현재 점수를 읽는 함수
	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const;
	
	// 점수를 추가해주는 함수
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
	
};
