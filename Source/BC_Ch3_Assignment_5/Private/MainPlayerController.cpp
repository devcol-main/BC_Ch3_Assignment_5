#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함
#include "MainGameState.h"
#include "MainGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

// 어차피 블루프린트 상에서 전부 다 초기화를 하기 때문에 여기서는 전부 다 nullptr 처리
AMainPlayerController::AMainPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr),
	  HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainMenuWidgetClass(nullptr),
	  MainMenuWidgetInstance(nullptr)

{
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// GetLocalPlayer():현재 PlayerController에 연결된 Local Player 객체를 가져옴     
	// Local Player 는 그 플레이어의 입력이나 화면 뷰 같은 것을 관리하는 어떤 객체
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
		// UEnhancedInputLocalPlayerSubsystem: 입력 시스템을 관리 (IMC 추가 혹은 삭제하는 역할)
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				// Subsystem을 통해 우리가 할당한 IMC를 활성화
				// 우선순위(Priority)는 0이 가장 높은 우선순위
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}


	/*// HUD 위젯 생성 및 표시
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}

	AMainGameState* MainGameState = GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
	if (MainGameState)
	{
		MainGameState->UpdateHUD();
	}*/
}

UUserWidget* AMainPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}


// 메뉴 UI 표시
void AMainPlayerController::ShowMainMenu(bool bIsRestart, bool bIsGameComplete )
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	// 메뉴 UI 생성
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(
			MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));			
				
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}
		
		if (bIsRestart)
		{
			UTextBlock* GameOverText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("GameOverText"));
			
			if (bIsGameComplete)
			{				
				if (GameOverText)
				{
					GameOverText->SetText(FText::FromString(TEXT("Game Complete")));
				}
			}
			else
			{				
				
			}
			
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %i"), MainGameInstance->TotalScore)
					));
				}
			}
			
			
		}
	}
}

// 게임 HUD 표시
void AMainPlayerController::ShowGameHUD()
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			AMainGameState* MainGameState = GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
			if (MainGameState)
			{
				MainGameState->UpdateHUD();
			}
		}
	}
}

// 게임 시작 - BasicLevel 오픈, GameInstance 데이터 리셋
void AMainPlayerController::StartGame()
{
	if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		MainGameInstance->CurrentLevelIndex = 0;
		MainGameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}
