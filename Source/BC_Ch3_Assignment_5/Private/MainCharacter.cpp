
#include "MainCharacter.h"

//
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "MainPlayerController.h"

// 카메라, 스프링 암 실제 구현이 필요한 경우라서 include
// 전방 선언(Forward Declaration) 한것 여기서 (실질적으로 사용하는곳) 포함
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 

//
#include "GameFramework/CharacterMovementComponent.h" // GetCharacterMovement() 사용을 위해


AMainCharacter::AMainCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	
	// (1) 스프링 암 생성
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// 스프링 암을 루트 컴포넌트 (CapsuleComponent)에 부착
	SpringArmComp->SetupAttachment(RootComponent);
	// 캐릭터와 카메라 사이의 거리 기본값 300으로 설정
	SpringArmComp->TargetArmLength = 300.0f;  
	// 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
	SpringArmComp->bUsePawnControlRotation = true;  
	
	
	//

	// (2) 카메라 컴포넌트 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 스프링 암의 소켓 위치에 카메라를 부착
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// 카메라는 스프링 암의 회전을 따르므로 PawnControlRotation은 꺼둠
	CameraComp->bUsePawnControlRotation = false;
	
	// Movements & Sprint Speed
	
	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.75f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	
	// #include "GameFramework/CharacterMovementComponent.h" 추가해야함
	// MaxWalkSpeed 변경시 캐릭터 이동속도가 즉시 변경
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 AMainPlayerController로 캐스팅
        if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
        {
        	// null check
            if (PlayerController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::Move
                );
            }
            
            if (PlayerController->JumpAction)
            {
                // IA_Jump 액션 키를 "키를 누르고 있는 동안" StartJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::StartJump
                );
                
                // IA_Jump 액션 키에서 "손을 뗀 순간" StopJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AMainCharacter::StopJump
                );
            }
            
            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AMainCharacter::Look
                );
            }
            
            if (PlayerController->SprintAction)
            {
                // IA_Sprint 액션 키를 "누르고 있는 동안" StartSprint() 호출
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,                   
                    this, 
                    &AMainCharacter::StartSprint
                );
                // IA_Sprint 액션 키에서 "손을 뗀 순간" StopSprint() 호출
                EnhancedInput->BindAction(
                    PlayerController->SprintAction, 
                    ETriggerEvent::Completed, 
                    this, 
                    &AMainCharacter::StopSprint
                );
            }    
        }
    }

}

void AMainCharacter::Move(const FInputActionValue& value)
{
	// 컨트롤러가 있어야 방향 계산이 가능
	if (!Controller) return;

	// Value는 Axis2D로 설정된 IA_Move의 입력값 (WASD)을 담고 있음
	// 예) (X=1, Y=0) → 전진 / (X=-1, Y=0) → 후진 / (X=0, Y=1) → 오른쪽 / (X=0, Y=-1) → 왼쪽
	const FVector2D MoveInput = value.Get<FVector2D>();

	// IsNearlyZero
	// 부동소수점들은 딱 0으로 안 떨어 질 수도 있기 때문에 작은 오차들은 0으로 처리 하기 위한 함수
	if (!FMath::IsNearlyZero(MoveInput.X))
	{		
		// 캐릭터가 바라보는 방향(정면)으로 X축 이동
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		// 캐릭터의 오른쪽 방향으로 Y축 이동
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AMainCharacter::Look(const FInputActionValue& value)
{
	// 마우스의 X, Y 움직임을 2D 축으로 가져옴
	FVector2D LookInput = value.Get<FVector2D>();

	// X는 좌우 회전 (Yaw), Y는 상하 회전 (Pitch)
	// 좌우 회전
	AddControllerYawInput(LookInput.X);
	
	// 상하 회전
	// IA에서 반전해둔 상태
	// 여기엔 추후 변경 가능한 옵션 추가해주자
	AddControllerPitchInput(LookInput.Y);
}

void AMainCharacter::StartJump(const FInputActionValue& value)
{
	// Jump 함수는 Character가 기본 제공
	if (value.Get<bool>())
	{
		Jump();
	}
}
void AMainCharacter::StopJump(const FInputActionValue& value)
{	
	// StopJumping 함수도 Character가 기본 제공
	if (!value.Get<bool>())
	{
		StopJumping();
	}	
}

void AMainCharacter::StartSprint(const FInputActionValue& value)
{
	// Shift 키를 누른 순간 이 함수가 호출된다고 가정
	// 스프린트 속도를 적용
	if (GetCharacterMovement())
	{
		// 중간에 변경시 
		SprintSpeed = NormalSpeed * SprintSpeedMultiplier;		
		
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

}
void AMainCharacter::StopSprint(const FInputActionValue& value)
{
	// Shift 키를 뗀 순간 이 함수가 호출
	// 평상시 속도로 복귀
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
