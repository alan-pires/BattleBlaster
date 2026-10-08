// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(capsuleComp);

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMash"));
	baseMesh->SetupAttachment(capsuleComp);

	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMash"));
	turretMesh->SetupAttachment(baseMesh);

	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("projectileSpawnPoint"));
	projectileSpawnPoint->SetupAttachment(turretMesh);
	

}

void ABasePawn::RotateTurret(FVector target)
{
	FVector toTarget = target - turretMesh->GetComponentLocation();
	FRotator lookAtRotation = FRotator(0.0f, toTarget.Rotation().Yaw, 0.0f);
	
	FRotator InterpolatedRotation = FMath::RInterpTo(turretMesh->GetComponentRotation(), lookAtRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
	turretMesh->SetWorldRotation(InterpolatedRotation);
}

void ABasePawn::Fire()
{
	FVector spawnLocation = projectileSpawnPoint->GetComponentLocation();
	FRotator spawnRotation = projectileSpawnPoint->GetComponentRotation();

	FVector offset = spawnRotation.Vector() * 10.0f;
	spawnLocation += offset;

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, spawnLocation, spawnRotation);

	if (projectile)
	{
		projectile->SetOwner(this);
	}
}

void ABasePawn::HandleDestruction()
{
	if (deathParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), deathParticles, GetActorLocation(), GetActorRotation());
	}
	if (deathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), deathSound, GetActorLocation());
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->ClientStartCameraShake(deathCameraShakeClass);
	}}


