// Fill out your copyright notice in the Description page of Project Settings.


#include "Atank.h"

Atank::Atank()
{
	armComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("ArmComp"));
	armComp->SetupAttachment(capsuleComp);

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(armComp);

	isAlive = true;
}

// Called when the game starts or when spawned
void Atank::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<APlayerController>(Controller);
	
	if (playerController)
	{
		ULocalPlayer* localPlayer = playerController->GetLocalPlayer();
		if (localPlayer)
		{	
			if (UEnhancedInputLocalPlayerSubsystem* playerSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer))
			{
				playerSubSystem->AddMappingContext(defaultMappingContext, 0);
			}
		}
	}

	SetPlayerEnabled(false);
}

// Called every frame
void Atank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerController)
	{
		FHitResult hitResult;
		playerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
		
		RotateTurret(hitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void Atank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &Atank::MoveInput);
		enhInputComponent->BindAction(turnAction, ETriggerEvent::Triggered, this, &Atank::TurnInput);
		enhInputComponent->BindAction(fireAction, ETriggerEvent::Started, this, &Atank::Fire);
	}
}

void Atank::MoveInput(const FInputActionValue& value)
{
	float inputValue = value.Get<float>();

	FVector worldDelta = GetActorForwardVector() * speed * inputValue * GetWorld()->GetDeltaSeconds();

	FHitResult hitResult;
	AddActorWorldOffset(worldDelta, true, &hitResult);

	// slide along the surface instead of stopping dead on a glancing hit
	if (hitResult.IsValidBlockingHit())
	{
		FVector slideDelta = FVector::VectorPlaneProject(worldDelta, hitResult.Normal) * (1.0f - hitResult.Time);
		AddActorWorldOffset(slideDelta, true);
	}
}

void Atank::TurnInput(const FInputActionValue& value)
{
	float inputValue = value.Get<float>();
	
	FRotator deltaRotation = FRotator(0.0, 0.0, 0.0);
	deltaRotation.Yaw = turnRate * inputValue * GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(deltaRotation, true);
}

void Atank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);
	isAlive = false;
}

void Atank::SetPlayerEnabled(bool enabled)
{
	if (playerController)
	{
		if (enabled) 
			EnableInput(playerController);
		else 
			DisableInput(playerController);

		playerController->bShowMouseCursor = enabled;
	}
}
