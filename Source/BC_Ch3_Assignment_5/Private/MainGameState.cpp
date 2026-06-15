#include "MainGameState.h"
#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AMainGameState::AMainGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	ItemToSpawn = 0;
	LevelDuration = 0.0f; // 한 레벨당 30초
	CurrentLevelIndex = 0;
	MaxLevels = 3;

	//
	WaveDuration = 0.0f;
	CurrentWaveIndex = 0;
	MaxWave = 3;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 첫 레벨부터 진행
	StartLevel();

	//UpdateHUD();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMainGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 AMainGameState::GetScore() const
{
	return Score;
}

void AMainGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			MainGameInstance->AddToScore(Amount);
		}
	}
}

void AMainGameState::StartWave()
{
	// ====
	++CurrentWaveIndex;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
	                                 FString::Printf(TEXT("Wave %i Start!"), CurrentWaveIndex));
	
	FName CurrentMapName = LevelMapNames[CurrentLevelIndex];

	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Yellow,
		FString::Printf(TEXT("Current Map: %s"), *CurrentMapName.ToString())
	);

	//WaveDuration = 10.0f;
	
	//SpawnVolume->SpawnFixedItemsForWave(CurrentWaveIndex);
	
	// =====
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	
	if (FoundVolumes.Num() > 0)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		if (SpawnVolume)
		{
			SpawnVolume->SpawnFixedItemsForWave(CurrentWaveIndex);

		}
	}
	
	/*
	for (TActorIterator<ASpawnVolume> It(GetWorld()); It; ++It)
{
	ASpawnVolume* SpawnVolume = *It;

	if (SpawnVolume)
	{
		SpawnVolume->SpawnFixedItemsForWave(CurrentWaveIndex);
		break;
	}
}
	 */
	
/*
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
*/
	
	// =====

	switch (CurrentWaveIndex)
	{
	case 1:
		{
			WaveDuration += 30.0f;
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Yellow,
				FString::Printf(TEXT("WaveTimeAdded : %f"), WaveDuration)
			);					
			
		}

		break;
	case 2:
		{
			WaveDuration += 20.0f;
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Yellow,
				FString::Printf(TEXT("WaveTimeAdded : %f"), WaveDuration)
			);			
			
			
		}

		break;
	case 3:
		{
			WaveDuration += 15.0f;
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Yellow,
				FString::Printf(TEXT("WaveTimeAdded : %f"), WaveDuration)
			);			
		}

	}
	
	

	if (CurrentMapName == "BasicLevel")
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
		                                 FString::Printf(TEXT("BasicLevel!")));
	}

	if (CurrentMapName == "AdvancedLevel")
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
		                                 FString::Printf(TEXT("AdvancedLevel!")));
	}


	if (CurrentMapName == "IntermediateLevel")
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
		                                 FString::Printf(TEXT("IntermediateLevel!")));
	}


	// update timer
	UpdateTimer(LevelTimerHandle, WaveDuration);
}


void AMainGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
		{
			MainPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			CurrentLevelIndex = MainGameInstance->CurrentLevelIndex;
		}
	}

	// 레벨 시작 시, 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	//const int32 ItemToSpawn = 40;
	//ItemToSpawn = 30;
	
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
								 FString::Printf(TEXT("Total FoundVolumes: %i"), FoundVolumes.Num()));

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}





	/*
	// 30초 후에 OnLevelTimeUp()가 호출되도록 타이머 설정
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AMainGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);	
	*/


	/*UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
	       CurrentLevelIndex + 1,
	       SpawnedCoinCount);*/


	StartWave();
}

void AMainGameState::OnLevelTimeUp()
{
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}

void AMainGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	/*UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
	       CollectedCoinCount,
	       SpawnedCoinCount)*/
	
	/*
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, 
		FColor::Blue,
		FString::Printf(TEXT("Coin Collected: %d | %d"), CollectedCoinCount, SpawnedCoinCount));
		*/
	
	// start wave2, wave3, 

	// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
	if (SpawnedCoinCount > 0 )
	{
		// leveup
		if (CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
		
		if (CurrentWaveIndex == 1)
		{
			if (CollectedCoinCount >= SpawnedCoinCount * 0.25f)
			{
				StartWave();
			}
		}
		else if (CurrentWaveIndex == 2)
		{
			if (CollectedCoinCount >= SpawnedCoinCount * 0.5f)
			{
				StartWave();
			}
		}

		
	}
	
	/*
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
	 */
}

void AMainGameState::EndLevel()
{
	// 타이머 해제
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	// 다음 레벨 인덱스로
	//CurrentLevelIndex++;


	if (SpawnedCoinCount > 0 && CollectedCoinCount < SpawnedCoinCount)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,TEXT("GG"));
		OnGameOver();

		return;
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			MainGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}


	// TODO: Game Complete 로
	if (CurrentLevelIndex >= MaxLevels)
	{
		//
		OnGameComplete();
		//OnGameOver();
		return;
	}

	// 레벨 맵 이름이 있다면 해당 맵 불러오기
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	/*
	else
	{
		// 맵 이름이 없으면 게임오버
		OnGameOver();
	}
	*/
}

void AMainGameState::OnGameComplete()
{
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	//

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
		{
			MainPlayerController->SetPause(true);
			MainPlayerController->ShowMainMenu(true, true);
		}
	}
}

void AMainGameState::OnGameOver()
{
	//
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	//

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
		{
			MainPlayerController->SetPause(true);
			MainPlayerController->ShowMainMenu(true);
		}
	}

	//UpdateHUD();
	//UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
}

void AMainGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController);
		if (MainPlayerController)
		{
			UUserWidget* HUDWidget = MainPlayerController->GetHUDWidget();
			if (HUDWidget)
			{
				// requires
				// #include "Components/TextBlock.h"
				// #include "Blueprint/UserWidget.h"

				//추후 변경 방안 
				// UPROPERTY(meta = (BindWidget))
				// class UButton* MyAwesomeButton;
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				//
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
						if (MainGameInstance)
						{
							ScoreText->SetText(
								FText::FromString(FString::Printf(TEXT("Score: %i"), MainGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(
						FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
				
				//
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					LevelIndexText->SetText(
						FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex)));
				}
				
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Coin"))))
				{
					LevelIndexText->SetText(
						FText::FromString(FString::Printf(TEXT("Coin: %i / %i"), CollectedCoinCount, SpawnedCoinCount)));
				}
			}
		}
	}
}

void AMainGameState::UpdateTimer(FTimerHandle& TimerHandle, float DeltaTime)
{
	if (GetWorldTimerManager().IsTimerActive(TimerHandle))
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
		                                 FString::Printf(TEXT("IsTimerActive")));
		                                 */

		float UpdatedTime = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
		UpdatedTime += DeltaTime;

		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&AMainGameState::OnLevelTimeUp,
			UpdatedTime,
			false

		);
	}
	else
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		                                 FString::Printf(TEXT("NO Timer")));
		                                 */

		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&AMainGameState::OnLevelTimeUp,
			DeltaTime,
			false

		);
	}
}
