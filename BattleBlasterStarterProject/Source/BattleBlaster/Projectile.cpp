// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(projectileMesh);

	projectileMovComp = CreateDefaultSubobject< UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	trailParticles = CreateDefaultSubobject< UNiagaraComponent>(TEXT("trailParticles"));
	trailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (launchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), launchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* myOwner = GetOwner();

	if (myOwner)
	{
		if (OtherActor)
		{
			UGameplayStatics::ApplyDamage(OtherActor, damage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());

			if (hitParticles)
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitParticles, GetActorLocation(), GetActorRotation());

			if (hitSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, GetActorLocation());
			
			if (hitCameraShakeClass)
			{
				APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (playerController)
				{
					playerController->ClientStartCameraShake(hitCameraShakeClass);
				}
			}
		}

	}
	Destroy();
}

