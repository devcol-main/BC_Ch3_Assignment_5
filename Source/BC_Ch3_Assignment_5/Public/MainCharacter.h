#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

// 미리 선언
// 전방 선언(Forward Declaration) 
class USpringArmComponent; // 스프링 암 관련 클래스 헤더
class UCameraComponent; // 카메라 관련 클래스 전방 선언
class UWidgetComponent;

// Enhanced Input에서 액션 값을 받을 때 사용하는 구조체
struct FInputActionValue;

UCLASS()
class BC_CH3_ASSIGNMENT_5_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	
	virtual void BeginPlay() override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp  = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget = nullptr;
	
	// 현재 체력을 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
		
	// 체력을 회복시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	
protected:
	
	// === Variables ===
	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float  MaxHealth;
	// 현재 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float  Health;
	
	// = Movements
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도 SprintSpeed= NormalSpeed * SprintSpeedMultiplier
	
	
	// =====	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
	
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
	
	
	// 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();
	
	void UpdateOverheadHP();

	// 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
	// 또는 AActor의 TakeDamage()를 오버라이드
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent, // Damage 관련 추가 정보 (예: Skill 속성에 따른 반응) 
		AController* EventInstigator, //데미지를 발생 시킨 주체
		AActor* DamageCauser // 데미지를 일으킨 오브젝트
		) override;
	
};
