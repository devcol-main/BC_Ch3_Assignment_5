#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.f;
	ExplosionDamage = 50.0f;
	ItemType = "Mine";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	//ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 
	ExplosionCollision->SetCollisionProfileName(CollisionProfile_OverlapAllDynamic);
	ExplosionCollision->SetupAttachment(Scene);
	//
	
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(Scene);
	OverheadWidget->SetWidgetSpace(EWidgetSpace::World);
	
	OverheadWidget->SetVisibility(false); 
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AMineItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!OverheadWidget)
		return;
	
	UpdateOverHeadWidget();
	
	
}

void AMineItem::UpdateOverHeadWidget()
{
	
	UpdateOverheadWidgetRotation();
	
	
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) 
		return;
	
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(ExplosionTimerHandle);
	
	if (UTextBlock* TimerText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("TimerText"))))
	{	
		TimerText->SetText(FText::FromString(FString::Printf(TEXT(" %.2f / %.2f"), RemainingTime, ExplosionDelay)));
	}
	
	if (UProgressBar* TimerProgressBar = Cast<UProgressBar>(OverheadWidgetInstance->GetWidgetFromName(TEXT("TimerProgressBar"))))
	{
		TimerProgressBar->SetPercent(RemainingTime/ExplosionDelay);
	}
	
}

void AMineItem::UpdateOverheadWidgetRotation()
{
	// 1. OverheadWidget 유효성 확인 (변수명이 OverheadWidget이고 컴포넌트인 경우)
	

	// World 유효성 확인
	UWorld* World = GetWorld();
	if (!World) return;

	// PlayerController 가져오기
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC) return;

	// PlayerCameraManager 가져오기
	APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
	if (!CameraManager) return;

	// 카메라 위치 변수 선언 및 가져오기
	FVector CameraLocation = CameraManager->GetCameraLocation();

	// 위젯 위치 변수 선언 및 가져오기
	FVector WidgetLocation = OverheadWidget->GetComponentLocation();

	// 위젯에서 카메라를 바라보는 방향 벡터 계산
	FVector LookAtVector = CameraLocation - WidgetLocation;

	// 방향 벡터를 회전값으로 변환
	FRotator TargetRotation = LookAtVector.Rotation();	
	//TargetRotation.Pitch = 0.0f;
	//TargetRotation.Roll = 0.0f;

	OverheadWidget->SetWorldRotation(TargetRotation);
}


void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;
	Super::ActivateItem(Activator);
	
	SetActorTickEnabled(true);// 

	// ExplosionDelay 후 폭발 실행
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
	
	bHasExploded = true;
	
	// UI
	if (!OverheadWidget) 
		return;
	
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) 
		return;
	
	OverheadWidget->SetVisibility(true); 	
	OverheadWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	
	
}

void AMineItem::Explode()
{
	SetActorTickEnabled(false);
	
	UParticleSystemComponent* Particle = nullptr;

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("ActivateItem Explode")));*/

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			//true // 여기서 true 면 auto destroy 되어야하는데 안된다면, particle이 loop 형신인거라 다른 추가 조치를 해주어야 합니다.
			false // 그래서 loop 나 세밀한 조정을 원한경우 따로 destroy 구현합니다
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}

	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player damaged %d by MineItem"), ExplosionDamage));

			// 데미지를 발생시켜 Actor->TakeDamage()가 실행되도록 함
			UGameplayStatics::ApplyDamage(
				Actor, // 데미지를 받을 액터
				ExplosionDamage, // 데미지 양
				nullptr, // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
				this, // 데미지를 유발한 오브젝트(지뢰)
				UDamageType::StaticClass() // 기본 데미지 유형
			);
		}
	}

	// 지뢰 제거
	DestroyItem();

	if (Particle)
	{
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
		FTimerHandle DestroyParticleTimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false
		);
		
		/*
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			// lamda: 익명 함수 (이름이 없는 함수)
			// [ ] 일종의 캡쳐리스트 람다 싱행시 [ ] 안에 있는 (현재는 Particle) 변수를 바깥 스코프에서 값을 가져다가 
			// 사용할 수 있게 만드는 것.
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			ExplosionDelay,
			//1.0f, // destroy time in sec
			false
		);
		*/
	}
}
