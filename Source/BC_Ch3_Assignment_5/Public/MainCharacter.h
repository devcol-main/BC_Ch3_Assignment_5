#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

// 미리 선언
// 전방 선언(Forward Declaration) 
class USpringArmComponent; // 스프링 암 관련 클래스 헤더
class UCameraComponent; // 카메라 관련 클래스 전방 선언


// Enhanced Input에서 액션 값을 받을 때 사용하는 구조체
struct FInputActionValue;

UCLASS()
class BC_CH3_ASSIGNMENT_5_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp  = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp = nullptr;
	


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

protected:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도 SprintSpeed= NormalSpeed * SprintSpeedMultiplier
	
	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//여기서 인자값을 참조로 받는 이유는 구조체는 크기때문에 참조를 안하고 직접 가져오면 복사비용이 커서 성능 저하 문제가 발생합니다
	// IA_Move와 IA_Jump 등을 처리할 함수 원형
    // Enhanced Input에서 액션 값은 FInputActionValue로 전달됩니다.
	// UFUNCTION()을 붙이지 않으면 바인딩에 실패할 수 있습니다.
	// 블루프린트 접근성을 설정하지 않았더라도, 기본적으로 메타데이터가 생성됩니다.
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	
	UFUNCTION()
	void Look(const FInputActionValue& value);
	
	// on off 의 형태의 것들은 그냥 나눠 주는 것이 좋다
	// 왜냐하면 이전에는 세세하게 처리하기가 상당이 까다로웠는데
	// EnhancedInputSystem은 그것들을 매우 편하게 변경해주었기 때문에 왠만하면 나눠 주는 것이 좋다
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	
};
