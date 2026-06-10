#include "MainPlayerController.h"

// Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함
#include "EnhancedInputSubsystems.h"

// 어차피 블루프린트 상에서 전부 다 초기화를 하기 때문에 여기서는 전부 다 nullptr 처리
AMainPlayerController::AMainPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr)

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
	
}

