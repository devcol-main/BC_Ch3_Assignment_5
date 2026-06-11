#include "BaseItem.h"
#include "Components/SphereComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;
    
	// 루트 컴포넌트 생성 및 설정
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// 충돌 컴포넌트 생성 및 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// 겹침만 감지하는 프로파일 설정
	//Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetCollisionProfileName(CollisionProfile_OverlapAllDynamic);
	// 루트 컴포넌트로 설정
	Collision->SetupAttachment(Scene);
    
	// 스태틱 메시 컴포넌트 생성 및 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	// 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정할 수 있음.

	// Overlap 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

void ABaseItem::OnItemOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult)
{
	
	/*
	추가 팁: 인터페이스(Interface) 활용하기만약 플레이어뿐만 아니라 '데미지를 입을 수 있는 모든 대상'을 판별하고 싶다면, 
	Cast 대신 언리얼 인터페이스(UInterface)를 사용해 OtherActor->Implements<UMyDamageInterface>() 형태로 판별하는 것이 아키텍처 관점에서 훨씬 깔끔합니다.
	
	// 싱글 플레이어 게임이거나 0번 로컬 플레이어를 판별할 때 가장 간단하고 직관적인 방법입니다. 충돌한 액터가 현재 월드의 플레이어 폰과 같은지 메모리 주소를 직접 비교합니다.
	// 단점: 멀티플레이어 환경(다른 클라이언트의 플레이어 캐릭터)에서는 작동하지 않을 수 있습니다.
	if (OtherActor && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{		
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("플레이어가 충돌했습니다!!!")));
	}  
	
	// 멀티플레이어 환경에서 적 플레이어, 아군 플레이어 구분 없이 '유저가 잡고 있는 캐릭터'를 모두 판별해 냅니다. 
	// AI 캐릭터(IsBotControlled)는 자동으로 걸러집니다.
	// #include "GameFramework/Pawn.h" // 다른 곳에서 선언된거랑 가능한한 여기서도 선언해두자
	if (OtherActor)
	{
		// 우선 APawn으로 캐스팅 (모든 캐릭터는 폰의 일종입니다)
		APawn* TouchedPawn = Cast<APawn>(OtherActor);
		
		// 폰이 맞고, 현재 플레이어 컨트롤러가 빙의(Possess)한 상태인지 확인
		if (TouchedPawn && TouchedPawn->IsPlayerControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("플레이어가 조종 중인 액터입니다!!!")));
		}
	}	
	
	// 플레이어와 AI를 동시에 분기 처리하기
	// 하나의 충돌 함수 안에서 플레이어와 AI를 나누어 각각 다른 로직
	// (예: 플레이어면 UI 연출, AI면 데미지만 처리)을 실행하고 싶다면 다음과 같이 작성합니다.
	
	if (OtherActor)
	{
		APawn* TouchedPawn = Cast<APawn>(OtherActor);
			if (TouchedPawn)
			{
				if (TouchedPawn->IsPlayerControlled())
				{
					// 실제 플레이어 전용 로직
					UE_LOG(LogTemp, Log, TEXT("플레이어 충돌"));
				}
				else if (TouchedPawn->IsBotControlled())
				{
					//  AI 봇 전용 로직
					UE_LOG(LogTemp, Log, TEXT("AI 캐릭터 충돌"));
				}
			}
	}

	 */
	
	
	// OtherActor가 플레이어인지 확인 ("Player" 태그 활용)
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		// 아이템 사용 (획득) 로직 호출
		ActivateItem(OtherActor);
	}
}

void ABaseItem::OnItemEndOverlap(
			UPrimitiveComponent* OverlappedComp, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex)
{
}

void ABaseItem::ActivateItem(AActor* Activator)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}