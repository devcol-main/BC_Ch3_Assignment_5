#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	//ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 
	ExplosionCollision->SetCollisionProfileName(CollisionProfile_OverlapAllDynamic);
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;


	Super::ActivateItem(Activator);

	// ExplosionDelay 후 폭발 실행
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);

	bHasExploded = true;
}

void AMineItem::Explode()
{
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
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Particle destroyed wid delay %f"), ExplosionDelay));
			*/


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
	}
}
