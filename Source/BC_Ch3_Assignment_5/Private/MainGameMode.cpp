#include "MainGameMode.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"

AMainGameMode::AMainGameMode()
{
	// StaticClass(): 클래스 이름을 통해서 호출해주는것
	DefaultPawnClass = AMainCharacter::StaticClass();
	//
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
